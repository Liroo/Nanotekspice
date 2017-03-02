#include <algorithm>

#include "NcursesMode.hpp"
#include "CLIException.hpp"

// So much work on this input user class

nts::Mode::NcursesMode::NcursesMode() {
  if ((_win = initscr()) == NULL) {
    throw nts::Exception::CLIException(ECLINCURSESINIT);
  }
  /*
    zsh doc:z
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

  /*
    map some keys and ctrl and shifted keys
    there is much unhandled keys noted here
    but there is many more printable keys... !
  */
  _cmdMap = {
    // handled keys which all have a different behavior
    { (int)(KEY_LEFT), [this]() -> void { this->_handleKeyLeft(); } },
    { (int)(KEY_RIGHT), [this]() -> void { this->_handleKeyRight(); } },
    { (int)(KEY_CLEFT), [this]() -> void { this->_handleKeyCtrlLeft(); } },
    { (int)(KEY_CRIGHT), [this]() -> void { this->_handleKeyCtrlRight(); } },
    { (int)(KEY_SOH), [this]() -> void { this->_handleKeyCtrlA(); } },
    { (int)(KEY_ENQ), [this]() -> void { this->_handleKeyCtrlE(); } },
    { (int)(KEY_BACKSPACE), [this]() -> void { this->_handleKeyDeleteCharacter(); } },
    { (int)(KEY_UP), [this]() -> void { this->_handleKeyHistoryForward(); } },
    { (int)(KEY_DOWN), [this]() -> void { this->_handleKeyHistoryBackward(); } },
    { (int)(KEY_EOT), [this]() -> void { this->_handleKeyCtrlD(); } },

    // Unhandled keys
    { (int)(KEY_LEFTTAB), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_BREAK), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_HOME), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_F0), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_EIC), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_CLEAR), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_NPAGE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_PPAGE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_STAB), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_PRINT), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_LL), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_A1), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_A3), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_B2), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_C1), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_C3), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_BTAB), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_COPY), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_FIND), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_MOUSE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_REDO), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_REFRESH), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_RESIZE), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_SAVE), [this]() -> void { this->_handleUnhandledKey(); } },

    { (int)(KEY_SBEG), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SCANCEL), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SCOMMAND), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SCOPY), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SCREATE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SDC), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SDL), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SELECT), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SEND), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SEOL), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SEXIT), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SFIND), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SHELP), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SHOME), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SIC), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SLEFT), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SMESSAGE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SMOVE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SNEXT), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SOPTIONS), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SPREVIOUS), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SPRINT), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SREDO), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SREPLACE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SRIGHT), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SSAVE), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SSUSPEND), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_SUNDO), [this]() -> void { this->_handleUnhandledKey(); } },
    { (int)(KEY_UNDO), [this]() -> void { this->_handleUnhandledKey(); } },
  };
  // init some attributes
  _inputCmd = "";
  nts::sout = new nts::Mode::NcursesOut(_win);
  nts::serr = nts::sout;
  // separator used for ctrl + left/right keys
  // use to find next step where cursor should stop
  _moveSeparator = { ' ', '=' };
}

nts::Mode::NcursesMode::~NcursesMode() {
  delete nts::sout;
  endwin();
}

std::string nts::Mode::NcursesMode::readCmd() {
  int inputChar = 0;

  // init input
  _inputCmdIndex = 0;
  _inputCmd = "";
  _readingInput = true;

  // init history filter
  _historyFilter = "\n"; // init by \n because this is impossible to create a filter with \n
  _historyFilterResultIndex = 0;
  _historyFilterResult.clear();
  _blockRefreshHistory = false;

  // prompt
  wprintw(_win, CLI_PROMPT);
  // loop for read a command until '\n'
  while (inputChar != KEY_NEWLINE && _readingInput) {
    // getChar
    inputChar = wgetch(_win);

    /*
      DEBUG Mode
    */
    //*nts::sout << inputChar << "\n";

    try {
      // is the input key is mapped with special function ?
      // otherwise, it goes to the normal handle in catch
      nts::Mode::NcursesMode::KeyBind keyBind;

      keyBind = _cmdMap.at(inputChar);
      keyBind();
    } catch (const std::out_of_range&) {
      // add key to the buffer and move the cursor
      _addKeyToBuffer(inputChar);
    }
  }
  // add the last command to the history
  // a command cannot be in vector two times in a row
  _addToHistory(_inputCmd);
  return _inputCmd;
}

void nts::Mode::NcursesMode::_changeBuffer(const std::string& str) {
  // move to the begining of the input
  std::pair<int, int> cursorPosition = _getCursorPosition();
  _moveCursorPosition(cursorPosition.first - _inputCmdIndex, cursorPosition.second);
  // rewrite str
  wprintw(_win, "%s", str.c_str());
  // and remove extra characters
  int extraCharToRemove = _inputCmd.size() - str.size();
  for (; extraCharToRemove >= 0; extraCharToRemove--) {
    wdelch(_win);
  }
  // assign buffer to cmd in our class
  _inputCmd = str;
  _inputCmdIndex = str.size();
  /*
    move back the cursor position to the inition value
    We can't use previous position because if the input is multipleline
    It's not guaranted the cursor will be at the same position
  */
  /*
    don't necessary use it
    old version, keep a trace of this if we need it
  */

  // cursorPosition = _getCursorPosition();
  // _moveCursorPosition(cursorPosition.first - (str.size() - _historyFilter.size()), cursorPosition.second);
  // _inputCmd = str;

  /*
    instead of the old version
    we need to force block refresh history
  */
  _blockRefreshHistory = true;
}

void nts::Mode::NcursesMode::_addKeyToBuffer(int inputChar) {
  // handle printable key which mean this is an char from the command
  // don't add new line to the command
  if (inputChar != KEY_NEWLINE) {
    std::string strInput("");
    const char *printableInput = unctrl(inputChar);
    strInput = printableInput;
    _inputCmd.insert(_inputCmdIndex, strInput);

    if ((int)_inputCmd.size() > COLS - CLI_PROMPT_SIZE - (int)strInput.size() + 1) {
      // create a substr to re write all the next input on multiple line
      std::string subStr = _inputCmd.substr(_inputCmdIndex);
      // write it
      wprintw(_win, "%s", subStr.c_str());

      // save cursor position
      std::pair<int, int> cursorPosition = _getCursorPosition();
      // move cursor to last position
      if (_moveCursorPosition(cursorPosition.first - subStr.size() + strInput.size(), cursorPosition.second)) {
        _inputCmdIndex += strInput.size();
      }
    } else {
      // print the character because of noecho() mode
      for (int i = 0; i < (int)strInput.size(); i++) {
        // insert char at correct position
        winsch(_win, strInput[i]);
        // then move the cursor to the right as a real cmd interpretor
        _handleKeyRight();
      }
    }
  } else {
    // print \n and refresh / flush the window
    // move to the end of the input
    std::pair<int, int> cursorPosition = _getCursorPosition();
    _moveCursorPosition(cursorPosition.first + (_inputCmd.size() - _inputCmdIndex), cursorPosition.second);
    *nts::sout << "\n";
    wrefresh(_win);
  }
  // unblock refresh history
  _blockRefreshHistory = false;
}

void nts::Mode::NcursesMode::_addToHistory(const std::string& lastCmd) {
  if (lastCmd == "") { return; }
  HistoryCmd::iterator lastCmdInHistory = _history.end();

  if (_history.size() > 0) {
    lastCmdInHistory--;
  }
  // if last cmd is the same as this one, don't add the cmd to history
  if (lastCmdInHistory != _history.end() && *lastCmdInHistory == lastCmd) {
    return;
  }
  // add the cmd to history
  _history.push_back(lastCmd);
}

bool nts::Mode::NcursesMode::_refreshHistoryFilter(const std::string& historyFilter) {
  // if filter is the same as before, don't touch the result
  if (historyFilter == _historyFilter) {
    return false;
  }
  // replace new filter
  _historyFilter = historyFilter;
  // create vector of the result of the research
  _historyFilterResult.clear();
  std::for_each(_history.begin(), _history.end(),
    [=](std::string historyItem) -> void {
      if (historyFilter == "" || historyItem.compare(0, historyFilter.length(), historyFilter) == 0) {
        _historyFilterResult.push_back(historyItem);
      }
    });
  // push back history cmd that we can get it by history and no force use when down is reached
  _historyFilterResult.push_back(_inputCmd);
  // set _historyFilterResultIndex to know where we are in the filter
  // put it at the last position
  _historyFilterResultIndex = _historyFilterResult.size() - 1;
  return true;
}

void nts::Mode::NcursesMode::_handleKeyLeft() {
  // if cursor is already on the first position of the string, return
  if (_inputCmdIndex < 1) {
    return;
  }
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first - 1, cursorPosition.second)) {
    _inputCmdIndex -= 1;
  }
  // unblock refresh history
  _blockRefreshHistory = false;
}

void nts::Mode::NcursesMode::_handleKeyRight() {
  // if cursor is already on the first position of the string, return
  if (_inputCmdIndex > (int)(_inputCmd.size()) - 1) {
    return;
  }
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first + 1, cursorPosition.second)) {
    _inputCmdIndex += 1;
  }
  // unblock refresh history
  _blockRefreshHistory = false;
}

void nts::Mode::NcursesMode::_handleKeyCtrlLeft() {
  int leftShifted = _inputCmdIndex;

  // pass throught all separator currently on
  while (leftShifted > 0 && _moveSeparator.find(_inputCmd[leftShifted]) != _moveSeparator.end()) {
    leftShifted--;
  }

  // loop until a moveSeparator is reached or line start
  while (leftShifted > 0 && _moveSeparator.find(_inputCmd[leftShifted]) == _moveSeparator.end()) {
    leftShifted--;
  }
  // calculate the difference
  leftShifted = _inputCmdIndex - leftShifted;

  // move correcty the cursor
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first - leftShifted, cursorPosition.second)) {
    _inputCmdIndex -= leftShifted;
  }

  // unblock refresh history
  _blockRefreshHistory = false;
}

void nts::Mode::NcursesMode::_handleKeyCtrlRight() {
  int leftShifted = _inputCmdIndex;

  // pass throught all separator currently on
  while (leftShifted < (int)_inputCmd.size() && _moveSeparator.find(_inputCmd[leftShifted]) != _moveSeparator.end()) {
    leftShifted++;
  }

  // loop until a moveSeparator is reached or line start
  while (leftShifted < (int)_inputCmd.size() && _moveSeparator.find(_inputCmd[leftShifted]) == _moveSeparator.end()) {
    leftShifted++;
  }
  // calculate the difference
  leftShifted -= _inputCmdIndex;

  // move correcty the cursor
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first + leftShifted, cursorPosition.second)) {
    _inputCmdIndex += leftShifted;
  }

  // unblock refresh history
  _blockRefreshHistory = false;
}

void nts::Mode::NcursesMode::_handleKeyCtrlA() {
  // move correcty the cursor at first position
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first - _inputCmdIndex, cursorPosition.second)) {
    _inputCmdIndex = 0;
  }
}

void nts::Mode::NcursesMode::_handleKeyCtrlE() {
  // move correcty the cursor at last position
  std::pair<int, int> cursorPosition = _getCursorPosition();
  if (_moveCursorPosition(cursorPosition.first + (_inputCmd.size() - _inputCmdIndex), cursorPosition.second)) {
    _inputCmdIndex = _inputCmd.size();
  }
}

void nts::Mode::NcursesMode::_handleKeyDeleteCharacter() {
  if (_inputCmdIndex < 1) {
    return;
  }
  _handleKeyLeft();
  if (_inputCmd.size() > 0) {
    _inputCmd.erase(_inputCmdIndex, 1);
    /*
      we could use same algorithm to delete a char inside the terminal
      but the way we do things is much slower on multiple lines
      Actually I use many libcall to ncurses when I have to rewrite
      multiple lines on the terminal
      it's O(N) speed on multiple lines where is O(1) on single
    */
    if ((int)_inputCmd.size() > COLS - CLI_PROMPT_SIZE - 1) {
      // create a substr to re write all the next input on multiple line
      std::string subStr = _inputCmd.substr(_inputCmdIndex);
      // write it
      wprintw(_win, "%s", subStr.c_str());
      // remove last extra char
      wdelch(_win);
      // save cursor position
      std::pair<int, int> cursorPosition = _getCursorPosition();
      // move cursor to last position
      _moveCursorPosition(cursorPosition.first - subStr.size(), cursorPosition.second);
    } else {
      wdelch(_win);
    }
  }
  // unblock refresh history
  _blockRefreshHistory = false;
}

void nts::Mode::NcursesMode::_handleKeyHistoryForward() {
  // refresh historyFilter if it change
  std::string actualFilter = _inputCmd.substr(0, _inputCmdIndex);

  // refresh here the filter if needed
  !_blockRefreshHistory && _refreshHistoryFilter(actualFilter);
  // here we want n - 1 item
  if (_historyFilterResultIndex - 1 >= 0) {
    _historyFilterResultIndex -= 1;
  }
  if (_historyFilterResult.empty()) {
    return;
  }
  _changeBuffer(_historyFilterResult[_historyFilterResultIndex]);
}

void nts::Mode::NcursesMode::_handleKeyHistoryBackward() {
  // refresh historyFilter if it change
  std::string actualFilter = _inputCmd.substr(0, _inputCmdIndex);

  // refresh here the filter if needed
  !_blockRefreshHistory && _refreshHistoryFilter(actualFilter);
  // here we want n + 1 item
  if (_historyFilterResultIndex + 1 < (int)(_historyFilterResult.size())) {
    _historyFilterResultIndex += 1;
  }
  if (_historyFilterResult.empty()) {
    return;
  }
  _changeBuffer(_historyFilterResult[_historyFilterResultIndex]);
}

void nts::Mode::NcursesMode::_handleKeyCtrlD() {
  // stop reading input anyway on ctrl-D
  _readingInput = false;
  if (_inputCmd.size() == 0) {
    // if there is no input registered
    // let's exit
    _inputCmd = "exit";
  } else {
    _addKeyToBuffer('\n');
  }
}

void nts::Mode::NcursesMode::_handleUnhandledKey() {
  return;
}

std::pair<int, int> nts::Mode::NcursesMode::_getCursorPosition() {
  // create pair variable which really is the position of the cursor on the screen
  std::pair<int, int> cursorPosition;
  // getyx macro to get cursorPosition
  getyx(_win, cursorPosition.second, cursorPosition.first);

  return cursorPosition;
}

bool nts::Mode::NcursesMode::_moveCursorPosition(int x, int y) {
  int xNew = 0;
  int yNew = 0;

  // if x < 0, back to the top line
  if (x < 0 && y > 0) {
    yNew = y - (((-x) / COLS) + 1);
    xNew = COLS - ((-x) % COLS);
  // if x > max COLS, go ahead to next line
  } else if (x > COLS - 1) {
    // because of the ncurses winsch, the cursor don't move automaticly
    // I have to add a newline manually to handle the scroll
    if (y == LINES - 1) {
      waddch(_win, '\n');
      yNew = y;
      xNew = 0;
    // or just go to the next line
    } else {
      yNew = y + (x / COLS);
      xNew = x % COLS;
    }
  } else {
    yNew = y;
    xNew = x;
  }
  // move the cursor to the position (xNew, yNew)
  if (wmove(_win, yNew, xNew) == ERR) {
    return false;
  }
  return true;
}

/*
  Some operator overloading of our output class for NcursesMode
  the class is extern (a bit like static). The class is accessible from anywhere in the program

  there is actually three operator overload:
    - std::string
    - char *
    - int

  useful to display many type on ncurses ui
*/
nts::Mode::IOut& nts::Mode::NcursesOut::operator<<(const std::string& str) {
  if (_win) {
    wprintw(_win, "%s", str.c_str());
  } else {
    printw("%s", str.c_str());
  }
  return *this;
}

nts::Mode::IOut& nts::Mode::NcursesOut::operator<<(const char* str) {
  if (_win) {
    wprintw(_win, "%s", str);
  } else {
    printw("%s", str);
  }
  return *this;
}

nts::Mode::IOut& nts::Mode::NcursesOut::operator<<(int i) {
  if (_win) {
    wprintw(_win, "%d", i);
  } else {
    printw("%d", i);
  }
  return *this;
}
