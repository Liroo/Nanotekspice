#ifndef NCURSESMODE_HPP
# define NCURSESMODE_HPP

# include <map>
# include <utility>
# include "IMode.hpp"
# include "ncurses.h"

// Extension of ncurses to know if inputChar is newline
# define KEY_NEWLINE 10

class nts::CLI::Mode::NcursesMode: public nts::CLI::Mode::IMode {
  public:
    NcursesMode();
    virtual ~NcursesMode();

  public:
    virtual std::string readCmd();

  private:
    int _inputCursorIndex;
    int _inputSize;
    std::string _inputCmd;
    WINDOW *_win;

  public:
    typedef std::function<void()> KeyBind;
  private:
    std::map<int, KeyBind> _cmdMap;

    void _handleKeyLeft();
    void _handleKeyRight();

  // ncurses help
  private:
    std::pair<int, int> _getCursorPosition();
    bool _moveCursorPosition(int, int);
};

#endif
