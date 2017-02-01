#include "Parser.hpp"

// Parser implementation
//
// a                   b                c
// compo++-------------+----------------+-------------->
// |                   |                |
// links+-------->a:1  |                |
//                 b:1 |                |
//                     |                |
//                links+----------->b:1 |
//                                  a:1 |
//                                ->b:2 |
//                                  c:1 |
//                                      |
//                                 links+--------->c:1
//                                                 b:2


nts::Parser::Parser() {
  _input = "";
  _ast = NULL;
}

nts::Parser::~Parser() {}

void nts::Parser::feed(std::string const& input) {
  _input += input;
}

void nts::Parser::parseTree(t_ast_node& root) {
  (void)root;
}

nts::t_ast_node* nts::Parser::createTree() {
  std::istringstream input(_input);
  std::string line;

  //ASTSectionType currentSection = nts::ASTSectionType::UNDEFINED;

  // loop file to store each line in `line` var and analyse it
  while (std::getline(input, line)) {
    std::cout << line << std::endl;
  }

  return _ast;
}
