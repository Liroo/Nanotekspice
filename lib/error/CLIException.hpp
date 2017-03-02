#ifndef CLIEXCEPTION_HPP
# define CLIEXCEPTION_HPP

# include "BaseException.hpp"

# define ECLIUSAGE "Usage: nanotekspice file [input=value, ...]"
# define ECLIACCESS "no such regular file"
# define ECLIBADSYNTAX "bad syntax"
# define ECLIUNKNOWOPT "unknown option"
# define ECLIARGNOTFOUND "variable not found"
# define ECLIARGNOTWRITABLE "is read only"

# define ECLINCURSESINIT "ncurses failed to init, switch to basic mode"

namespace nts {
  namespace Exception {
    class CLIException: public BaseException {
      public:
        CLIException(const std::string &msg)
          : BaseException(std::cerr, msg) {};
        CLIException(const std::string &msg, const int &ret)
          : BaseException(std::cerr, msg) {
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
