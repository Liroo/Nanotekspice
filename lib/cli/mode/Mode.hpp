#ifndef MODE_HPP
# define MODE_HPP

# include <iostream>
# include <string>
# include "ncurses.h"

# define CLI_PROMPT "> "
# define CLI_PROMPT_SIZE 2

namespace nts {
  // definition of mode namespace like of mode of CLI
  class Mode {
    public:
      class AMode;
      class NcursesMode;
      class BasicMode;
      /**/

      class IOut {
        public:
          virtual ~IOut() {};

        public:
          virtual IOut& operator<<(const std::string&) = 0;
          virtual IOut& operator<<(const char*) = 0;
          virtual IOut& operator<<(int) = 0;
      };
      class NcursesOut: public IOut {
        public:
          NcursesOut(WINDOW *win) {
            _win = win;
          }

        public:
          virtual IOut& operator<<(const std::string&);
          virtual IOut& operator<<(const char *);
          virtual IOut& operator<<(int);

        private:
          WINDOW *_win;
      };
      class BasicOut: public IOut {
        public:
          BasicOut(std::ostream& os): _os(os) {}

        public:
          virtual IOut& operator<<(const std::string&);
          virtual IOut& operator<<(const char *);
          virtual IOut& operator<<(int);

        private:
          std::ostream& _os;
      };
      /**/
  };
  extern nts::Mode::IOut *sout;
  extern nts::Mode::IOut *serr;
};

#endif
