#include <iostream>
#include "CLI.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {
  // exit is expected here if there not right params
  nts::CLI cli(argc, argv);
  
  cli.startCLI();
  return 0;
}
