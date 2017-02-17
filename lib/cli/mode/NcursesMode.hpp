#ifndef NCURSESMODE_HPP
# define NCURSESMODE_HPP

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
    WINDOW *_win;
};

#endif
