#ifndef BASEEXCEPTION_HPP
# define BASEEXCEPTION_HPP

# include <exception>
# include <iostream>
# include <string>

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
  };
};

#endif
