#ifndef BASICMODE_HPP
# define BASICMODE_HPP

# include "Mode.hpp"

# include "AMode.hpp"

class nts::Mode::BasicMode: public nts::Mode::AMode {
  public:
    BasicMode();
    virtual ~BasicMode();

  public:
    virtual std::string readCmd();

};

#endif
