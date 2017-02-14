#ifndef BASEEXCEPTION_HPP
# define BASEEXCEPTION_HPP

# include <exception>
# include <iostream>
# include <string>

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
    class BaseException: public std::exception {
      public:
        BaseException(std::ostream &, const std::string &);
        virtual ~BaseException() throw();

      protected:
        std::ostream& _os;
        std::string _msg;

      public:
        const char *what() const throw();
        std::ostream& getOs() const;
    };
    class PARSERException : public BaseException {
      public:
        PARSERException(std::ostream &os, const std::string &msg)
          : BaseException(os, msg) {};
    };
  }
};

#endif
