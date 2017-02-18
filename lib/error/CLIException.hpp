#ifndef CLIEXCEPTION_HPP
# define CLIEXCEPTION_HPP

# include "BaseException.hpp"

# define ECLIUSAGE "Usage: nanotekspice file [input=value, ...]"
# define ECLIACCESS "no such regular file"
# define ECLIBADSYNTAX "bad syntax"
# define ECLIUNKNOWOPT "unknown option"

# define ECLINCURSESINIT "ncurses failed to init, switch to basic mode"

namespace nts {
  namespace Exception {
    class CLIException: public BaseException {
      public:
        CLIException(std::ostream &os, const std::string &msg)
          : BaseException(os, msg) {};
        CLIException(std::ostream &os, const std::string &msg, const int &ret)
          : BaseException(os, msg) {
            _ret = ret;
          };

        public:
          int getReturn() const {
            return _ret;
          }

        private:
          int _ret;
    };
  };
};

#endif
