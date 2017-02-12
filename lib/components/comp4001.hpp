#ifndef COMP4001_HPP
# define COMP4001_HPP

namespace nts {
  class C4001;
};

# include "AComponent.hpp"

class nts::C4001 : public nts::AComponent {
  public:
    C4001(const std::string &name);
    virtual ~C4001() {};
};

#endif /* COMP4001_HPP */
