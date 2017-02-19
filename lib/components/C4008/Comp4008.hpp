#ifndef COMP4008_HPP
# define COMP4008_HPP

namespace nts {
  class C4008;
};

# include "AComponent.hpp"

class nts::C4008 : public nts::AComponent {
  public:
    C4008(const std::string &name);
    virtual ~C4008() {};
};

#endif /* COMP4001_HPP */
