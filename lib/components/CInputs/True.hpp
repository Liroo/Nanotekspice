#ifndef TRUE_HPP
# define TRUE_HPP

# include "AComponent.hpp"

namespace nts {
  class True;
};

class nts::True : public nts::AComponent {
  public:
    True(const std::string &name);
    virtual ~True() {};
};

#endif /* TRUE_HPP */
