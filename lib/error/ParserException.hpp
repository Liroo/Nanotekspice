#ifndef PARSEREXCEPTION_HPP
# define PARSEREXCEPTION_HPP

# include "BaseException.hpp"

# define EPARSINVALIDSECTION "invalid section"
# define EPARSBADSYNTAX "bad syntax"
# define EPARSARGBADTYPE "invalid type: should be input or clock"
# define EPARSARGNOTFOUND "variable not found"
# define EPARSARGEXISTS "variable already exists"
# define EPARSARGNOTSET "input variable not initialized"
# define EPARSMISSINGCHIPSETS "Missing chipsets section or chipset variable"
# define EPARSMISSINGLINKS "Missing links section or link"

namespace nts {
  namespace Exception {
    class ParserException: public BaseException {
      public:
        ParserException(std::ostream &os, const std::string &msg)
          : BaseException(os, msg) {};
    };
  }
};

#endif
