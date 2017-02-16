#ifndef CLIEXCEPTION_HPP
# define CLIEXCEPTION_HPP

# include "BaseException.hpp"

# define ECLIUSAGE "Usage: nanotekspice file [input=value, ...]"
# define ECLIACCESS "no such regular file"
# define ECLIBADSYNTAX "bad syntax"
# define ECLIUNKNOWOPT "unknown option"

namespace nts {
  namespace Exception {
    class CLIException: public BaseException {
      public:
        CLIException(std::ostream &os, const std::string &msg)
          : BaseException(os, msg) {};
    };
  };
};

#endif
