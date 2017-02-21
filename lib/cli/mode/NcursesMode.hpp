#ifndef NCURSESMODE_HPP
# define NCURSESMODE_HPP

# include <map>
# include <vector>
# include <utility>
# include "AMode.hpp"
# include "ncurses.h"

// Extension of ncurses to know if inputChar is newline
# define KEY_NEWLINE 10

class nts::CLI::Mode::NcursesMode: public nts::CLI::Mode::AMode {
  public:
    NcursesMode();
    virtual ~NcursesMode();

  // read input Command method
  public:
    virtual std::string readCmd();

  // ncurses and input attributes
  private:
    bool _readingInput;
    int _inputCmdIndex;
    std::string _inputCmd;

  // keyBinding
  public:
    typedef std::function<void()> KeyBind;
  private:
    // basic function to add key to buffer and move correctly the cursor
    void _changeBuffer(const std::string&);
    void _addKeyToBuffer(int);

    std::map<int, KeyBind> _cmdMap;

    void _handleKeyLeft();
    void _handleKeyRight();
    void _handleKeyDeleteCharacter();
    void _handleKeyHistoryForward();
    void _handleKeyHistoryBackward();
    void _handleKeyCtrlD();

    void _handleUnhandledKey();

  // history stuff
  private:
    typedef std::vector<std::string> HistoryCmd;
    HistoryCmd _history;

    void _addToHistory(const std::string&);

    // filter feature on search
    // really useful when you whant to get back a command fast
    std::string _historyFilter;
    int _historyFilterResultIndex;
    HistoryCmd _historyFilterResult;

    bool _refreshHistoryFilter(const std::string&);


  // ncurses help
  private:
    WINDOW *_win;

    std::pair<int, int> _getCursorPosition();
    bool _moveCursorPosition(int, int);
};

class nts::CLI::Mode::NcursesOut: public nts::CLI::Mode::IOut {
  public:
    NcursesOut(WINDOW *win) {
      _win = win;
    }

  public:
    virtual IOut& operator<<(const std::string&);
    virtual IOut& operator<<(const char *);
    virtual IOut& operator<<(int);

  private:
    WINDOW *_win;
};

#endif
