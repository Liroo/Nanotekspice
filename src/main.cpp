#include <iostream>
#include "CLI.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {
  // exit is expected here if there not right params
  nts::CLI cli(argc, argv);
  nts::Parser parser(argv[1]);

  parser.createTree();
  cli.startCLI();
  return 0;
}
