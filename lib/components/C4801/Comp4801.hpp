#ifndef COMP4801_HPP
# define COMP4801_HPP

namespace nts {
  class C4801;
};

# include "AComponent.hpp"

class nts::C4801 : public nts::AComponent {
  public:
    C4801(const std::string &name);
    virtual ~C4801() {};
};

#endif /* COMP4801_HPP */
