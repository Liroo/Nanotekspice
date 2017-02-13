#ifndef CLIEXCEPTION_HPP
# define CLIEXCEPTION_HPP

# include "BaseException.hpp"

# define ECLIUSAGE "Usage: nanotekspice file [input=value, ...]"
# define ECLIACCESS "no such file or directory"
# define ECLIBADSYNTAX "bad syntax"

namespace nts {
  namespace Exception {
    class CLIException: public BaseException {
      public:
        CLIException(std::ostream &os, const std::string &msg)
          : BaseException(os, msg) {};
    };
  }
};

#endif
