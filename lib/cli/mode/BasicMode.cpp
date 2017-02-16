#include "BasicMode.hpp"

#include <iostream>

nts::CLI::Mode::BasicMode::BasicMode() {}

nts::CLI::Mode::BasicMode::~BasicMode() {}

std::string nts::CLI::Mode::BasicMode::readCmd() {
  // Prompt
  std::clog << CLI_PROMPT;
  // get user input
  // if user input is empty and EOF is reached, exit
  std::string input;
  if (!std::getline(std::cin, input) && input.empty()) {
    std::cout << "exit" << std::endl;
    input = "exit";
  }

  // Check if stdin stream is in error state or not
  // it could be caused by an.. EOF (ctrl-D)...
  // WELL TRIED RAPH, WELL TRIED !
  if (!std::cin) {
    std::cin.clear();
  }
  return input;
}
