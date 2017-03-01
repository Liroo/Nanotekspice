#ifndef COMP4017_HPP
# define COMP4017_HPP

namespace nts {
  class C4017;
};

# include "AComponent.hpp"

class nts::C4017 : public nts::AComponent {
  public:
    C4017(const std::string &name);
    virtual ~C4017() {};
};

#endif /* COMP4017_HPP */
