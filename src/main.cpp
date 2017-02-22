#include "CLI.hpp"

int main(int argc, char *argv[]) {
  // exit is expected here if there not right params
  nts::CLI *cli;

  try {
    // construct CLI using new operator
    // new is used because of the scope
    cli = new nts::CLI(argc, argv);
  } catch (const nts::Exception::CLIException& e) {
    // print error and return
    // I was using exit before but exit does not handle destructor
    std::cerr << e.what() << std::endl;
    return e.getReturn();
  }
  try {
    cli->startCLI();
  } catch (const nts::Exception::CLIException& e) {
    if (cli) {
      delete cli;
    }
    // print error and return
    // I was using exit before but exit does not handle destructor
    std::cerr << e.what() << std::endl;
    return e.getReturn();
  }
  if (cli) {
    delete cli;
  }
  return 0;
}
