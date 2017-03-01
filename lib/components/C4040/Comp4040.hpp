#ifndef COMP4040_HPP
# define COMP4040_HPP

namespace nts {
  class C4040;
};

# include "AComponent.hpp"

class nts::C4040 : public nts::AComponent {
  public:
    C4040(const std::string &name);
    virtual ~C4040() {};
};

#endif /* COMP4040_HPP */
