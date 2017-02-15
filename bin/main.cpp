#include "CLI.hpp"

int main(int argc, char *argv[]) {
  // exit is expected here if there not right params
  nts::CLI cli(argc, argv);

  return cli.startCLI();
}
