#ifndef COMP4081_HPP
# define COMP4081_HPP

namespace nts {
  class C4081;
};

# include "AComponent.hpp"

class nts::C4081 : public nts::AComponent {
  public:
    C4081(const std::string &name);
    virtual ~C4081() {};
};

#endif /* COMP4081_HPP */
