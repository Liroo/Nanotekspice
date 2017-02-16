#ifndef IMODE_HPP
# define IMODE_HPP

# include <string>
# include "CLI.hpp"

class nts::CLI::Mode::IMode {
  public:
    virtual ~IMode() {};

  public:
    virtual std::string readCmd() = 0;
};

#endif
