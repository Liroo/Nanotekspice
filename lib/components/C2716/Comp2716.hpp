#ifndef COMP2716_HPP
# define COMP2716_HPP

namespace nts {
  class C2716;
};

# include "AComponent.hpp"

class nts::C2716 : public nts::AComponent {
  public:
    C2716(const std::string &name);
    virtual ~C2716() {};
};

#endif /* COMP2716_HPP */
