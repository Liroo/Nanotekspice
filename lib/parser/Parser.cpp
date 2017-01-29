#include "Parser.hpp"

nts::Parser::Parser() {
  _input = "";
}

nts::Parser::~Parser() {}

void nts::Parser::feed(std::string const& input) {
  _input += input;
}

void nts::Parser::parseTree(t_ast_node& root) {
  (void)root;
}

nts::t_ast_node* nts::Parser::createTree() {
  return NULL;
}
