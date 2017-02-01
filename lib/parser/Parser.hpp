#ifndef PARSER_CPP
# define PARSER_CPP

# include <sstream>
# include <utility>

# include "IParser.hpp"
# include "iostream"

namespace nts {
  class Parser {
    private:
      std::string _input;
      nts::t_ast_node* _ast;

    public:
      Parser();
      virtual ~Parser();

    public:
      virtual void feed(std::string const& input);
      virtual void parseTree(t_ast_node& root);
      virtual t_ast_node *createTree();

    private:
      bool isSection(const std::string&);
      std::pair<std::string,
                std::string> getChipset(const std::string&);
      std::pair<std::string,
                std::string> getLink(const std::string&);
  };
};

#endif
