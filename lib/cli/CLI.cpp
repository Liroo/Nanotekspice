# include "CLI.hpp"

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

  std::smatch matched;
  std::regex regInput(REG_INPUTCLI);
  std::string line;

  // forEach like where goal is to parse arguments
  // if there is one false argument (bad syntax), it throw an exception that exit software
  for (int i = 2; argv[i]; i++) {
    line = argv[i];
    std::regex_search(line.cbegin(), line.cend(), matched, regInput);
    _config.inputValue.push_back(std::pair<std::string, std::string>(matched[1], matched[2]));
  }
} catch (const nts::Exception::CLIException& e) {
  // get output stream and print error
  e.getOs() << e.what() << std::endl;
  // exit here because we are in main constructor of the software
  exit(0);
}

nts::CLI::~CLI() {}

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
