#ifndef COMP4030_HPP
# define COMP4030_HPP

namespace nts {
  class C4030;
};

# include "AComponent.hpp"

class nts::C4030 : public nts::AComponent {
  public:
    C4030(const std::string &name);
    virtual ~C4030() {};
};

#endif /* COMP4030_HPP */
