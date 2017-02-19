#ifndef BASICMODE_HPP
# define BASICMODE_HPP

# include "AMode.hpp"

class nts::CLI::Mode::BasicMode: public nts::CLI::Mode::AMode {
  public:
    BasicMode();
    virtual ~BasicMode();

  public:
    virtual std::string readCmd();

};

class nts::CLI::Mode::BasicOut: public nts::CLI::Mode::IOut {
  public:
    BasicOut(std::ostream& os): _os(os) {}

  public:
    virtual IOut& operator<<(const std::string&);
    virtual IOut& operator<<(const char *);
    virtual IOut& operator<<(int);

  private:
    std::ostream& _os;
};

#endif
