#ifndef CLI_HPP
# define CLI_HPP

# include <vector>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include "CLIException.hpp"
#include "regex.h"

namespace nts {
  class CLI {
    public:
      CLI(int argc, char *argv[]);
      ~CLI();

    public:
      void startCLI();

    /* Public member function that is not really public ;) */
    public:
      void exitCLI();
      void display() const;
      void inputModifier();
      void simulate();
      void loop();
      void dump() const;

    private:
      struct config {
        std::string fileInput;
        std::vector<std::pair<std::string, std::string> > inputValue;
      };
      config _config;
  };
};

#endif