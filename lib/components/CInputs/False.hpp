#ifndef FALSE_HPP
# define FALSE_HPP

# include "AComponent.hpp"

namespace nts {
  class False;
};

class nts::False : public nts::AComponent {
  public:
    False(const std::string &name);
    virtual ~False() {};
};

#endif /* FALSE_HPP */
