#ifndef COMP4013_HPP
# define COMP4013_HPP

namespace nts {
  class C4013;
};

# include "AComponent.hpp"

class nts::C4013 : public nts::AComponent {
  public:
    C4013(const std::string &name);
    virtual ~C4013() {};
};

#endif /* COMP4013_HPP */
