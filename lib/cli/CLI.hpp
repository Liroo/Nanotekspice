#ifndef CLI_HPP
# define CLI_HPP

# include <vector>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <functional>
# include "CLIException.hpp"
# include "Parser.hpp"

# define REG_INPUTCLI "^(\\w+)=([01])"
# define CLI_CMD_NOT_FOUND "command not found. try `help`"
# define CLI_PROMPT "> "


/*
  nts::CLI(int argc, char *argv[]);

  CLI is complex class that could handle user input and basic command
  Basic command is associated to CLI as an unit

  help is display as below, you should be able to look up what command exist:
  nanotekspice:
    exit              -    exit nanotekspice
    help              -    show help
    display           -    display
    simulate          -    simulate
    loop              -    loop
    dump              -    dump
    ^(\\w+)=([01])    -    inputModifier
*/
namespace nts {
  class CLI {
    public:
      /*
        Constructor of CLI
        pass main arguments here, CLI should be able to detect
        - nts file
        - inputValue

        constructor is also used to init basic command
      */
      CLI(int argc, char *argv[]);
      // dcstr
      ~CLI();

    public:
      // startCLI will run CLI, waiting for user input and reading cmd
      void startCLI();

    /* Public member function that is not really public ;) */
    public:
      // exit the CLI
      bool exitCLI();
      // display help
      bool help();
      // TODO Doc
      bool display() const;
      // TODO Doc
      bool simulate();
      // TODO Doc
      bool loop();
      // TODO Doc
      bool dump() const;

    private:
      /*
        useful function to know if there is inputModifier in the string passed as parameter
        If string is an inputModifier, input value is modified
        otherwisem exception is thrown
      */
      void _extractInputValue(const std::string &arg);

    private:
      /*
        command map definition

        readCmd is the main function to read userInput and call correct function
        architecture of this CLI part is not the smartest, we could move some logic
        out of this class, or create new intermediate class, etc...
        but this one is Simple and efficient!

        We create an map of functionm each key represent a command, each value linked a function
        exit is linked to exit() function
        display is linked to display() function
        and so on...

        When a user input is detected, we gonna first check inputValue regex to know if there is inputModifier
        then we check the commands references in our map
        if none of these checks passed, we show "command not found" or something like this

        (_readCmd read only one line)

        Efficient.
      */
      typedef std::function<bool()> _CLICommand;
      std::map<std::string, _CLICommand> _cmd;
      bool _readCmd();

    private:
      /*
        private config of CLI class

        config is an JS object like represented as a struct in c++
        fileInput: string -> content of file passed as parameter
        inputValue: vector -> extracted inputModifier of argv params or CLI
      */
      struct config {
        std::string fileInput;
        std::vector<std::pair<std::string, std::string> > inputValue;
      };
      config _config;
  }; // namespace nts
};

#endif
