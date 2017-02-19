#ifndef COMP4011_HPP
# define COMP4011_HPP

namespace nts {
  class C4011;
};

# include "AComponent.hpp"

class nts::C4011 : public nts::AComponent {
  public:
    C4011(const std::string &name);
    virtual ~C4011() {};
};

#endif /* COMP4011_HPP */
