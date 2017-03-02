#ifndef COMP4514_HPP
# define COMP4514_HPP

namespace nts {
  class C4514;
};

# include "AComponent.hpp"

class nts::C4514 : public nts::AComponent {
  public:
    C4514(const std::string &name);
    virtual ~C4514() {};
};

#endif /* COMP4514_HPP */
