#ifndef BASICMODE_HPP
# define BASICMODE_HPP

# include "IMode.hpp"

class nts::CLI::Mode::BasicMode: public nts::CLI::Mode::IMode {
  public:
    BasicMode();
    virtual ~BasicMode();

  public:
    virtual std::string readCmd();
};

#endif
