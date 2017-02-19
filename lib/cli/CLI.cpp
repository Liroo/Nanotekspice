#include <sys/stat.h>
#include <regex>

#include "CLI.hpp"
#include "BasicMode.hpp"
#include "NcursesMode.hpp"

/*
  Init CLI by:
  - extracting file content
  - parse next arguments in argv list and set them in config
*/
nts::CLI::CLI(int argc, char *argv[]) try {
  if (argc == 1) {
    // error if there is no file argument
    throw nts::Exception::CLIException(std::cout, ECLIUSAGE);
  }

  std::ifstream fileInput;
  // set exceptions to throw if open fail, it automatise the is_open()
  fileInput.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::ostringstream out;
  try {
    // Check if this file is a regular file and not a directory
    struct stat buf;
    // if stat fail or file is not a regular file, throw error
    if (stat(argv[1], &buf) != 0 || !S_ISREG(buf.st_mode)) {
      throw std::ifstream::failure(ECLIACCESS);
    }

    // open file and throw exception if there is an error
    fileInput.open(argv[1], std::ifstream::in);

    // extract content of file in tmp var
    out << fileInput.rdbuf();
    // convert file content in _config.fileInput
    _config.fileInput = out.str();
  } catch (std::ifstream::failure e) {
    // convert ifstream exception in CLIException understood by the software
    throw nts::Exception::CLIException(std::cerr, std::string(argv[1]) + ": " + ECLIACCESS);
  }

  // forEach like where goal is to parse arguments
  // if there is one false argument (bad syntax), it throw an exception that exit software
  _config.mode = NULL;
  for (int i = 2; argv[i]; i++) {
    // regex for options
    std::smatch matched;
    std::regex regexOptions(REG_OPTIONS);
    std::string arg = std::string(argv[i]);

    // test regex options to know if the argument is an option
    std::regex_search(arg, matched, regexOptions);
    if (matched.size() == 3) {
      // handle mode option
      if (matched[1].compare("mode") == 0) {
        if (matched[2].compare("ncurses") == 0) {
          try {
            // if constructor of ncurses init failed, switch to basic Mode
            _config.mode = new nts::CLI::Mode::NcursesMode();
          } catch (nts::Exception::CLIException) {
            _config.mode = NULL;
          }
        }
      } else {
        throw nts::Exception::CLIException(std::cerr, std::string(argv[i]) + ": " + ECLIUNKNOWOPT);
      }
    } else {
    // Extract InputValue from argument here, throw an error if therte is a bad syntax
      _extractInputValue(arg);
    }
  }

  /*
    Init function pointer for CLI
    We was searching a badass way to create function pointer on c++11 with Sammy
    and we found this, it looks cool, using lambda function and map
    So in reading of command we can direct using command from map without searching it,
    just access his value by name
  */
  _cmd = {
    { "exit", [this]() -> bool { return this->exitCLI(); } },
    { "help", [this]() -> bool { return this->help(); } },
    { "display", [this]() -> bool { return this->display(); } },
    { "simulate", [this]() -> bool { return this->simulate(); } },
    { "loop", [this]() -> bool { return this->loop(); } },
    { "dump", [this]() -> bool { return this->dump(); } }
  };
  // Init dirty optimization
  _dirty = true;
  if (!_config.mode) {
    _config.mode = new nts::CLI::Mode::BasicMode();
  }
} catch (const nts::Exception::CLIException& e) {
  // get output stream and re catch error with correct exit code
  throw nts::Exception::CLIException(e.getOs(), e.what(), 1);
}

nts::CLI::~CLI() {
  if (_config.mode) {
    delete _config.mode;
  }
}

int nts::CLI::startCLI() {
  // WE SHALL MOVE SOME OF THIS LOGIC INTO SIMULATE,
  // feed parser should be handle in main or cstr and
  // we shall call simulate before startCLI in main

   std::istringstream input(_config.fileInput);
   std::string line;
   nts::t_ast_node *root;

   while (std::getline(input, line)) {
     _parser.feed(line + "\n");
   }
   try {
     root = _parser.createTree();
     // should check if every input/clock has a value
     _parser.parseTree(*root);
   } catch (nts::Exception::ParserException &e) {
     // show the message then return
     e.getOs() << e.what() << std::endl;
     return 1;
   }
   _comps = _parser.getComponentsMap();
   simulate();

  // loop reading command
  while (_execCmd()) {}
  return 0;
}

bool nts::CLI::_execCmd() {
  std::string input = _config.mode->readCmd();

  /*
    Check if input is an inputModifier.
    If it does, input is modified and function return
    If not, continue
  */
  bool isInputModifier = true;
  // remove whitespace and tabulation so we should be able to compare string using regex or direct access
  input.erase(std::remove(input.begin(), input.end(), '\t'), input.end());
  input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
  try {
    // Is regex to match extractInputValue is ok ?
    _extractInputValue(input);
  } catch (const nts::Exception::CLIException&) { // otherwise, CLIException is thrown
    // let's toggle this boolean to know there is no match with regex
    isInputModifier = false;
  }
  // if there is a match, return the function and ask for new cmd
  if (isInputModifier) {
    return true;
  }

  std::function<bool()> cmd;
  try {
    // is cmd referenced in out dictionary of cmd ?
    // if not, exception is throw
    cmd = _cmd.at(input);
  } catch (const std::out_of_range&) {
    // cmd not found, ask user to type help
    std::clog << input << ": " << CLI_CMD_NOT_FOUND << std::endl;
  }
  // cmd is found, so let's try to run it and return him :)
  if ((bool)cmd) {
    return cmd();
  }
  return true;
}

bool nts::CLI::exitCLI() {
  return false;
}

bool nts::CLI::help() {
  std::clog << "nanotekspice:" << std::endl
            << "  exit              -    exit nanotekspice" << std::endl
            << "  help              -    show help" << std::endl
            << "  display           -    display" << std::endl
            << "  simulate          -    simulate" << std::endl
            << "  loop              -    loop" << std::endl
            << "  dump              -    dump" << std::endl
            << "  ^(\\w+)=([01])    -    inputModifier" << std::endl
            ; // THIS SEMI COLON need to stay here, in case cmd is added in the future
  return true;
}

bool nts::CLI::display() const {
  // For each output, use the method dump
  std::for_each(_comps.begin(), _comps.end(),
  [](const std::pair<std::string, nts::IComponent *> &comp){
    if ((comp.second)->getType() == "output") {
      (comp.second)->Dump();
    }
  });
  // Then ouput newline
  std::cout << std::endl;
  return true;
}

void nts::CLI::_setDirty(const bool &dirty) {
  _dirty = dirty;
}

bool nts::CLI::_isDirty() const {
  return _dirty == true;
}

bool nts::CLI::simulate() {
  // TODO Doc
if (!_isDirty()) { return true; }

// TODO edit clock
_parser.setInputValues(_config.inputValue);
// reset pins compute security
std::for_each(_comps.begin(), _comps.end(),
  [](std::pair<std::string, nts::IComponent *> comp) {
    (comp.second)->resetPins();
});

std::for_each(_comps.begin(), _comps.end(),
[](const std::pair<std::string, nts::IComponent *> &comp) {
  std::cout << "Simulate on comp " << (comp.second)->getName() << std::endl;

  std::vector<nts::FlowChart *> gates = (comp.second)->getGates();

  std::for_each(gates.begin(), gates.end(),
  [&comp](const nts::FlowChart *gate) {
    std::vector<nts::Pin *> *outPins = gate->getOutputs();

    std::cout << "compute pin id " << (*outPins)[0]->getID() << std::endl;
    (comp.second)->Compute((*outPins)[0]->getID());
  });

});

//  reverse clocks' value
std::for_each(_comps.begin(), _comps.end(),
  [](std::pair<std::string, nts::IComponent *> comp){
    if ((comp.second)->getType() == "clock") {
      nts::Pin *pin = (comp.second)->getPins()[1];

      pin->setState((nts::Tristate)(!pin->getState()));
    }
  });
_setDirty(false);
return true;

}

bool nts::CLI::loop() {
  // Should loop simulate :)
  std::cout << "loop" << std::endl;

  return true;
}

bool nts::CLI::dump() const {
  // for each component, call method dump
  std::for_each(_comps.begin(), _comps.end(),
  [](const std::pair<std::string, nts::IComponent *> &comp){
    (comp.second)->Dump();
  });
  // Then output newline
  std::cout << std::endl;
  return true;
}


void nts::CLI::_extractInputValue(const std::string &arg) {
  std::regex regexInput(REG_INPUTCLI);
  std::smatch matched;

  // Regex Search for input name and value
  std::regex_search(arg, matched, regexInput);
  // If there is no 3 arguments (every group, input name, input value = 0 | 1), throw an error
  if (matched.size() != 3) {
    throw nts::Exception::CLIException(std::cerr, arg + ": " + ECLIBADSYNTAX);
  }

  /*
    Simple algorythm using lambda c++11 to find if input name is already categorized
    If it is, the value will change without remove and create a new node
    If not, it will push back this new input value
  */
  std::string inputNameMatched = matched[1];
  std::vector<std::pair<std::string, std::string> >::iterator it =
    std::find_if(_config.inputValue.begin(), _config.inputValue.end(),
    [&inputNameMatched](const std::pair<std::string, std::string> &item) -> bool {
      return (item.first.compare(inputNameMatched) == 0);
    });
  // if input exist, change his value
  if (it != _config.inputValue.end()) {
    (*it).second = matched[2];
  } else { // if not, create a new one
    _config.inputValue.push_back(std::pair<std::string, std::string>(matched[1], matched[2]));
  }
  _setDirty(true);
}
