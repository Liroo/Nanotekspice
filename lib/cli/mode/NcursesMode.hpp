#ifndef NCURSESMODE_HPP
# define NCURSESMODE_HPP

# include "IMode.hpp"

class nts::CLI::Mode::NcursesMode: public nts::CLI::Mode::IMode {
  public:
    NcursesMode();
    virtual ~NcursesMode();

  public:
    virtual std::string readCmd();
};

#endif
