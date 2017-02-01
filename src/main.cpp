#include <iostream>
#include "CLI.hpp"

int main(int argc, char *argv[]) {
  // crash is expected here if there not right params
  nts::CLI cli(argc, argv);

  cli.startCLI();
  return 0;
}
