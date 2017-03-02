#include "BasicMode.hpp"
#include <iostream>

nts::Mode::BasicMode::BasicMode() {
  nts::sout = new nts::Mode::BasicOut(std::cout);
  nts::serr = new nts::Mode::BasicOut(std::cerr);
}

nts::Mode::BasicMode::~BasicMode() {
  delete nts::sout;
  delete nts::serr;
}

std::string nts::Mode::BasicMode::readCmd() {
  // Prompt
  std::cout << CLI_PROMPT;
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


nts::Mode::IOut& nts::Mode::BasicOut::operator<<(const std::string& str) {
  _os << str;
  return *this;
}

nts::Mode::IOut& nts::Mode::BasicOut::operator<<(const char* str) {
  _os << str;
  return *this;
}

nts::Mode::IOut& nts::Mode::BasicOut::operator<<(int i) {
  _os << i;
  return *this;
}
