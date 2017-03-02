#ifndef AMODE_HPP
# define AMODE_HPP

# include <string>
# include "Mode.hpp"

class nts::Mode::AMode {
  public:
    virtual ~AMode() {};

  public:
    virtual std::string readCmd() = 0;
};

#endif
