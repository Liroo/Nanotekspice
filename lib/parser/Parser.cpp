#include "Parser.hpp"

nts::Parser::Parser() {
    std::cout << "Parser" << std::endl;
}

nts::Parser::~Parser() {
  std::cout << "Bye Parser" << std::endl;
}

void nts::Parser::feed(std::string const& input) {
  (void)input;
}

void nts::Parser::parseTree(t_ast_node& root) {
  (void)root;
}

nts::t_ast_node* nts::Parser::createTree() {
  return NULL;
}
