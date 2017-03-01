#ifndef COMP4069_HPP
# define COMP4069_HPP

namespace nts {
  class C4069;
};

# include "AComponent.hpp"

class nts::C4069 : public nts::AComponent {
  public:
    C4069(const std::string &name);
    virtual ~C4069() {};
};

#endif /* COMP4069_HPP */
