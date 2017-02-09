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


nts::Parser::Parser(const std::string &fileName) {
  _fileName = fileName;
  _input = "";
  _ast = NULL;
}

nts::Parser::~Parser() {}

nts::t_ast_node *nts::Parser::createNode(const std::string &lexeme,
                                          const nts::ASTNodeType &type,
                                          const std::string &value) const {
  nts::t_ast_node *newNode = new nts::t_ast_node(new std::vector<nts::t_ast_node *>);
  newNode->lexeme = lexeme;
  newNode->type = type;
  newNode->value = value;
  newNode->children = new std::vector<struct s_ast_node*>;
  return newNode;
}

void nts::Parser::initTree() {
  _ast = this->createNode("root", nts::ASTNodeType::DEFAULT);
  _ast->children->push_back(this->createNode("components list", nts::ASTNodeType::COMPONENT));
  _ast->children->push_back(this->createNode("link list", nts::ASTNodeType::LINK));
  _ast->children->push_back(this->createNode("link_end list", nts::ASTNodeType::LINK_END));
}

std::string nts::Parser::parseFile(const std::string &fileName) const {
  std::ifstream fs;
  std::ostringstream stream;

  fs.open(fileName);
  if (fs.is_open()) {
    stream << fs.rdbuf();
    fs.close();
  }
  return stream.str();
}

bool nts::Parser::checkSection(nts::ASTSectionType &currentSection, const std::string &line) const {
  std::smatch matched;
  std::regex regSect(REG_SECTION);

  std::regex_search(line.cbegin(), line.cend(), matched, regSect);
  if (matched.size() > 0) {
    if (matched[1] == "links") { currentSection = nts::ASTSectionType::LINK; }
    else if (matched[1] == "chipsets") { currentSection = nts::ASTSectionType::CHIPSET; }
    else {
      currentSection = nts::ASTSectionType::UNDEFINED;
      //throw;
    }
    return true;
  }
  return false;
}

void nts::Parser::checkWrongSection(const std::string &line, const nts::ASTSectionType &currentSection) const {
  (void)line;
  (void)currentSection;
}

void nts::Parser::addChipset(const std::string &line) {
  (*_ast->children)[0]->children->push_back(this->createNode(REG_CHIPSET, nts::ASTNodeType::COMPONENT, line));
}

void nts::Parser::addLink(const std::string &line) {
  std::smatch matched;
  std::regex regLinks(REG_LINKS);

  std::regex_search(line.cbegin(), line.cend(), matched, regLinks);
  (*_ast->children)[1]->children->push_back(this->createNode(REG_LINK, nts::ASTNodeType::LINK, matched[1]));
  (*_ast->children)[2]->children->push_back(this->createNode(REG_LINK, nts::ASTNodeType::LINK_END, matched[2]));
}

nts::t_ast_node* nts::Parser::createTree() {
  std::istringstream input(this->parseFile(_fileName));
  std::string line;
  ASTSectionType currentSection = nts::ASTSectionType::UNDEFINED;
  std::smatch matched;
  std::regex regCom(REG_COM);
  std::regex regEmpty(REG_EMPTY);

  this->initTree();
  // loop file to store each line in `line` var and analyse it
  while (std::getline(input, line)) {
    std::cout << line << std::endl;
    if (!this->checkSection(currentSection, line) &&
        !std::regex_match(line, regCom) && !std::regex_match(line, regEmpty)) {
        this->checkWrongSection(line, currentSection);
        if (currentSection == nts::ASTSectionType::CHIPSET) { this->addChipset(line); }
        else { this->addLink(line); }
      }
    }
  return _ast;
}

void nts::Parser::feed(std::string const& input) {
  _input += input;
}

void nts::Parser::parseTree(t_ast_node& root) {
  (void)root;
}
