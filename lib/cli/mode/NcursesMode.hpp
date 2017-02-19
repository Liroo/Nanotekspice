#ifndef NCURSESMODE_HPP
# define NCURSESMODE_HPP

# include <map>
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
    int _inputCursorIndex;
    std::string _inputCmd;
    WINDOW *_win;

  // keyBinding
  public:
    typedef std::function<void()> KeyBind;
  private:
    std::map<int, KeyBind> _cmdMap;

    void _handleKeyLeft();
    void _handleKeyRight();
    void _handleKeyDeleteCharacter();

  // ncurses help
  private:
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
