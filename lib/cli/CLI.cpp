#include "CLI.hpp"

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
  for (int i = 2; argv[i]; i++) {
    // Extract InputValue from argument here, throw an error if therte is a bad syntax
    extractInputValue(std::string(argv[i]));
  }

  /*
    Init function pointer for CLI
    We was searching a badass way to create function pointer on c++11 with Sammy
    and we found this, it looks cool, using lambda function and map
    So in reading of command we can direct using command from map without searching it,
    just access his value by name
  */
  _cmd = {
    { "exit", [this]() { this->exitCLI(); } },
    { "display", [this]() { this->display(); } },
    { "inputModifier", [this]() { this->inputModifier(); } },
    { "simulate", [this]() { this->simulate(); } },
    { "loop", [this]() { this->loop(); } },
    { "dump", [this]() { this->dump(); } }
  };
} catch (const nts::Exception::CLIException& e) {
  // get output stream and print error
  e.getOs() << e.what() << std::endl;
  // exit here because we are in main constructor of the software
  exit(0);
}

nts::CLI::~CLI() {
  // everything should be deleted automaticly :')
}

void nts::CLI::startCLI() {
  std::istringstream input(_config.fileInput);
  std::string line;
  nts::Parser parser;
  nts::t_ast_node *root;

  while (std::getline(input, line)) {
    parser.feed(line + "\n");
  }
  root = parser.createTree();
  // should check if every input/clock has a value
  parser.parseTree(*root);
  parser.setInputValues(_config.inputValue);
}

void nts::CLI::exitCLI() {}

void nts::CLI::display() const {}

void nts::CLI::inputModifier() {}

void nts::CLI::simulate() {
  

}

void nts::CLI::loop() {}

void nts::CLI::dump() const {}


void nts::CLI::extractInputValue(const std::string &arg) {
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
}
