#ifndef PARSER_CPP
# define PARSER_CPP

# include "IParser.hpp"
# include "iostream"

namespace nts {
  class Parser {
    private:
      std::string _input;

    public:
      Parser();
      virtual ~Parser();

    public:
      virtual void feed(std::string const& input);
      virtual void parseTree(t_ast_node& root);
      virtual t_ast_node *createTree();
  };
};

#endif
