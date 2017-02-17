#include "NcursesMode.hpp"

nts::CLI::Mode::NcursesMode::NcursesMode() {
  if ((_win = initscr()) == NULL) {
    throw nts::Exception::CLIException(std::clog, ECLINCURSESINIT);
  }
  /*
    zsh doc:
    We use cbreak mode because we don't want line buffering
    on input since we'd just need to loop over characters.
    We use noecho since the manual says that's the right
    thing to do with cbreak.

    Turn these on immediately to catch typeahead.
	 */
  cbreak();
  noecho();
  keypad(_win, true);
}

nts::CLI::Mode::NcursesMode::~NcursesMode() {
  delwin(_win);
}

std::string nts::CLI::Mode::NcursesMode::readCmd() {
  std::string inputCmd;
  int inputChar = 0;

  wprintw(_win, CLI_PROMPT);
  while (inputChar != KEY_NEWLINE) {
    inputChar = wgetch(_win);
    wprintw(_win, "%c", inputChar);
  }
  return "";
}
