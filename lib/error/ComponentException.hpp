#ifndef COMPONENTEXCEPTION_HPP
# define COMPONENTEXCEPTION_HPP

# include "BaseException.hpp"

namespace nts {
  namespace Exception {
    class ComponentException: public BaseException {
      public:
        ComponentException(std::ostream &os, const std::string &msg)
          : BaseException(os, msg) {};
    };
  };
};

#endif
