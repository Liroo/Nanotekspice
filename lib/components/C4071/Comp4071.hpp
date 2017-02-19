#ifndef COMP4071_HPP
# define COMP4071_HPP

namespace nts {
  class C4071;
};

# include "AComponent.hpp"

class nts::C4071 : public nts::AComponent {
  public:
    C4071(const std::string &name);
    virtual ~C4071() {};
};

#endif /* COMP4071_HPP */
