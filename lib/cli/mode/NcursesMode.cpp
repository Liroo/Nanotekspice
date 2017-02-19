#include "NcursesMode.hpp"

// So much work on this input user class

nts::CLI::Mode::NcursesMode::NcursesMode() {
  if ((_win = initscr()) == NULL) {
    throw nts::Exception::CLIException(ECLINCURSESINIT);
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
  // this line made many things <3
  scrollok(_win, true);

  _cmdMap = {
    { (int)(KEY_LEFT), [this]() -> void { this->_handleKeyLeft(); } },
    { (int)(KEY_RIGHT), [this]() -> void { this->_handleKeyRight(); } },
    { (int)(KEY_BACKSPACE), [this]() -> void { this->_handleKeyDeleteCharacter(); } }
  };
  // init some attributes
  _inputCmd = "";
  nts::sout = new nts::CLI::Mode::NcursesOut(_win);
  nts::serr = nts::sout;
}

nts::CLI::Mode::NcursesMode::~NcursesMode() {
  delete nts::sout;
  endwin();
}

std::string nts::CLI::Mode::NcursesMode::readCmd() {
  int inputChar = 0;

  // init input
  _inputCursorIndex = 0;
  _inputCmd = "";
  // prompt
  wprintw(_win, CLI_PROMPT);
  // loop for read a command until '\n'
  while (inputChar != KEY_NEWLINE) {
    // getChar
    inputChar = wgetch(_win);

    try {
      // is the input key is mapped with special function ?
      // otherwise, it goes to the normal handle in catch
      nts::CLI::Mode::NcursesMode::KeyBind keyBind;

      keyBind = _cmdMap.at(inputChar);
      keyBind();
    } catch (const std::out_of_range&) {
      // handle printable key which mean this is an char from the command
      // don't add new line to the command
      if (inputChar != KEY_NEWLINE) {
        std::string strInput("");
        const char *keylol = unctrl(inputChar);
        strInput = keylol;
        _inputCmd.insert(_inputCursorIndex, strInput);
        // print the character because of noecho() mode
        for (int i = 0; i < (int)strInput.size(); i++) {
          winsch(_win, strInput[i]);
          _handleKeyRight();
        }
      } else {
        *nts::sout << "\n";
      }
    }
  }
  return _inputCmd;
}

void nts::CLI::Mode::NcursesMode::_handleKeyLeft() {
  // if cursor is already on the first position of the string, return
  if (_inputCursorIndex < 1) {
    return;
  }
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first - 1, cursorPosition.second)) {
    _inputCursorIndex -= 1;
  }
}

void nts::CLI::Mode::NcursesMode::_handleKeyRight() {
  // if cursor is already on the first position of the string, return
  if (_inputCursorIndex > (int)(_inputCmd.size()) - 1) {
    return;
  }
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first + 1, cursorPosition.second)) {
    _inputCursorIndex += 1;
  }
}

void nts::CLI::Mode::NcursesMode::_handleKeyDeleteCharacter() {
  _handleKeyLeft();
  if (_inputCmd.size() > 0) {
    _inputCmd.erase(_inputCursorIndex, 1);
    if ((int)_inputCmd.size() > COLS - CLI_PROMPT_SIZE - 1) {
      // save cursor position
      std::pair<int, int> cursorPosition = _getCursorPosition();
      // create a substr to re write all the next input on multiple line
      std::string subStr = _inputCmd.substr(_inputCursorIndex);
      // write it
      mvwprintw(_win, cursorPosition.second, cursorPosition.first, "%s", subStr.c_str());
      // remove last extra char
      wdelch(_win);
      // move cursor to last position
      wmove(_win, cursorPosition.second, cursorPosition.first);
    } else {
      wdelch(_win);
    }
  }
}

std::pair<int, int> nts::CLI::Mode::NcursesMode::_getCursorPosition() {
  // create pair variable which really is the position of the cursor on the screen
  std::pair<int, int> cursorPosition;
  // getyx macro to get cursorPosition
  getyx(_win, cursorPosition.second, cursorPosition.first);

  return cursorPosition;
}

bool nts::CLI::Mode::NcursesMode::_moveCursorPosition(int x, int y) {
  int xNew = 0;
  int yNew = 0;
  if (x < 0 && y > 0) {
    yNew = y - 1;
    xNew = COLS - 1;
  } else if (x > COLS - 1) {
    yNew = y + 1;
    xNew = 0;
  } else {
    yNew = y;
    xNew = x;
  }
  if (wmove(_win, yNew, xNew) == ERR) {
    return false;
  }
  return true;
}

nts::CLI::Mode::IOut& nts::CLI::Mode::NcursesOut::operator<<(const std::string& str) {
  if (_win) {
    wprintw(_win, "%s", str.c_str());
  } else {
    printw("%s", str.c_str());
  }
  return *this;
}

nts::CLI::Mode::IOut& nts::CLI::Mode::NcursesOut::operator<<(const char* str) {
  if (_win) {
    wprintw(_win, "%s", str);
  } else {
    printw("%s", str);
  }
  return *this;
}

nts::CLI::Mode::IOut& nts::CLI::Mode::NcursesOut::operator<<(int i) {
  if (_win) {
    wprintw(_win, "%d", i);
  } else {
    printw("%d", i);
  }
  return *this;
}
