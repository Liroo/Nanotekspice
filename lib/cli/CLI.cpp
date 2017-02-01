# include "CLI.hpp"

nts::CLI::CLI(int argc, char *argv[]) try {
  if (argc == 1) {
    throw nts::Exception::CLIException(std::cout, ECLIUSAGE);
  }
  (void) argv;
} catch (const nts::Exception::CLIException& e) {
  e.getOs() << e.what() << std::endl;
  exit(0);
}

nts::CLI::~CLI() {}

void nts::CLI::startCLI() {}
