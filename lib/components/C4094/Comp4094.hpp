#ifndef COMP4094_HPP
# define COMP4094_HPP

namespace nts {
  class C4094;
};

# include "AComponent.hpp"

class nts::C4094 : public nts::AComponent {
  public:
    C4094(const std::string &name);
    virtual ~C4094() {};
};

#endif /* COMP4094_HPP */
