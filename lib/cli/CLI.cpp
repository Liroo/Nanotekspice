#include <sys/stat.h>
#include <regex>
#include "signal.h"

#include "CLI.hpp"
#include "BasicMode.hpp"
#include "NcursesMode.hpp"

namespace nts {
  nts::CLI::Mode::IOut *sout = NULL;
  nts::CLI::Mode::IOut *serr = NULL;
  bool CLI::isLooping = false;
};

/*
  Init CLI by:
  - extracting file content
  - parse next arguments in argv list and set them in config
*/
nts::CLI::CLI(int argc, char *argv[]) try {
  if (argc == 1) {
    // error if there is no file argument
    throw nts::Exception::CLIException(ECLIUSAGE);
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
    throw nts::Exception::CLIException(std::string(argv[1]) + ": " + ECLIACCESS);
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
        throw nts::Exception::CLIException(std::string(argv[i]) + ": " + ECLIUNKNOWOPT);
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
  // If Mode is already define, delete it to be sure terminal is well reset
  if (_config.mode) {
    delete _config.mode;
  }
  // get output stream and re catch error with correct exit code
  throw nts::Exception::CLIException(e.what(), 1);
}

nts::CLI::~CLI() {
  // delete Mode to reset terminal and free memory
  if (_config.mode) {
    delete _config.mode;
  }
}

int nts::CLI::startCLI() {
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
     *nts::serr << e.what() << "\n";
     return 1;
   }
   _comps = _parser.getComponentsMap();
   try {
     simulate();
   } catch (const nts::Exception::BaseException &e) {
     *nts::serr << e.what() << "\n";
     return 1;
   }

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
  // trim whitespace left, right and double in
  input.erase(std::find_if(input.rbegin(), input.rend(),
    std::not1(std::ptr_fun<int, int>(std::isspace))).base(), input.end());
  input.erase(input.begin(), std::find_if(input.begin(), input.end(),
    std::not1(std::ptr_fun<int, int>(std::isspace))));
  std::string::iterator new_end = std::unique(input.begin(), input.end(),
    [](char lhs, char rhs) -> bool { return (lhs == rhs) && (lhs == ' '); });
  input.erase(new_end, input.end());

  if (input == "") {
    return true;
  }
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
    // cmd is found, so let's try to run it and return him :)
    bool cmdReturn = true;

    try {
      cmdReturn = cmd();
    } catch (const nts::Exception::BaseException &e) {
      *nts::serr << e.what() << "\n";
    }
    return cmdReturn;
  } catch (const std::out_of_range&) {
    // cmd not found, ask user to type help
    *nts::sout << input << ": " << CLI_CMD_NOT_FOUND << "\n";
  }
  return true;
}

bool nts::CLI::exitCLI() {
  return false;
}

bool nts::CLI::help() {
  *nts::serr << "nanotekspice:" << "\n"
            << "  exit              -    exit nanotekspice" << "\n"
            << "  help              -    show help" << "\n"
            << "  display           -    display" << "\n"
            << "  simulate          -    simulate" << "\n"
            << "  loop              -    loop" << "\n"
            << "  dump              -    dump" << "\n"
            << "  ^(\\w+)=([01])    -    inputModifier" << "\n"
            ; // THIS SEMI COLON need to stay here, in case cmd is added in the future
  return true;
}

bool nts::CLI::display() const {
  // For each output, use the method dump
  std::for_each(_comps.begin(), _comps.end(),
  [](const std::pair<std::string, nts::IComponent *> &comp){
    if ((comp.second)->getType() == "output") {
      *nts::sout << comp.first << "=" << (comp.second)->getPins()[1]->getState() << "\n";
    }
  });
  return true;
}

void nts::CLI::_setDirty(const bool &dirty) {
  _dirty = dirty;
}

bool nts::CLI::_isDirty() const {
  return _dirty == true;
}

bool nts::CLI::simulate() {
  _parser.setInputValues(_config.inputValue);

  //  function to reverse clock's value
  std::function<void(std::pair<std::string, std::string> &inputValue)> uploadClock =
      [this](std::pair<std::string, std::string> &inputValue)->void{
        std::string name(inputValue.first);
        int value;
        std::stringstream(inputValue.second) >> value;
        std::map<std::string, nts::IComponent *>::iterator it;

        it = std::find_if(_comps.begin(), _comps.end(),
          [&name](const std::pair<std::string, nts::IComponent *> comp) {
            return (comp.second)->getName() == name && (comp.second)->getType() == "clock";
          });
        if (it != _comps.end()) {
            inputValue.second = std::to_string(!value);
            ((*it).second)->uploadRising();
          }
        };

  if (!_isDirty()) {
    std::for_each(_config.inputValue.begin(), _config.inputValue.end(), uploadClock);
    return true;
  }

  // reset pins compute security
  std::for_each(_comps.begin(), _comps.end(),
    [](std::pair<std::string, nts::IComponent *> comp) {
      (comp.second)->resetPins();
  });

  //  compute components
  std::for_each(_comps.begin(), _comps.end(),
  [](const std::pair<std::string, nts::IComponent *> &comp) {
    std::vector<nts::FlowChart *> gates = (comp.second)->getGates();

    std::for_each(gates.begin(), gates.end(),
    [&comp](const nts::FlowChart *gate) {
      std::vector<nts::Pin *> *outPins = gate->getOutputs();

      (comp.second)->Compute((*outPins)[0]->getID());
    });
  });

  std::for_each(_config.inputValue.begin(), _config.inputValue.end(), uploadClock);
  _setDirty(false);
  return true;
}

bool nts::CLI::loop() {
  nts::CLI::isLooping = true;

  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = [](int sig) -> void {
    (void)sig;
    nts::CLI::isLooping = false;
  };
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  while (isLooping) {
    simulate();
  }
  sigIntHandler.sa_handler = SIG_DFL;
  sigaction(SIGINT, &sigIntHandler, NULL);

  return true;
}

bool nts::CLI::dump() const {
  // for each component, call method dump
  std::for_each(_comps.begin(), _comps.end(),
  [](const std::pair<std::string, nts::IComponent *> &comp){
    (comp.second)->Dump();
  });
  // Then output newline
  *nts::sout << "\n";
  return true;
}


void nts::CLI::_extractInputValue(const std::string &arg) {
  std::regex regexInput(REG_INPUTCLI);
  std::smatch matched;

  // Regex Search for input name and value
  std::regex_search(arg, matched, regexInput);
  // If there is no 3 arguments (every group, input name, input value = 0 | 1), throw an error
  if (matched.size() != 3) {
    throw nts::Exception::CLIException(arg + ": " + ECLIBADSYNTAX);
  }

  // Check if input exist
  if (!_comps.empty()) {
    std::map<std::string, nts::IComponent *>::iterator inputFound = _comps.find(matched[1]);
    if (inputFound == _comps.end()) {
      *nts::sout << std::string(matched[1]) << ": " << ECLIARGNOTFOUND << "\n";
      return;
    } else if ((*inputFound).second->getType() != "input") {
      *nts::sout << std::string(matched[1]) << ": " << ECLIARGNOTWRITABLE << "\n";
      return;
    }
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
