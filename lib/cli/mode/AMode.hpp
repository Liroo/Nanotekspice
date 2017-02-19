#ifndef AMODE_HPP
# define AMODE_HPP

# include <string>
# include "CLI.hpp"

class nts::CLI::Mode::AMode {
  public:
    virtual ~AMode() {};

  public:
    virtual std::string readCmd() = 0;
};

class nts::CLI::Mode::IOut {
  public:
    virtual ~IOut() {};

  public:
    virtual IOut& operator<<(const std::string&) = 0;
    virtual IOut& operator<<(const char*) = 0;
    virtual IOut& operator<<(int) = 0;
};

#endif
