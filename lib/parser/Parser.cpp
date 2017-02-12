#include "Parser.hpp"

nts::Parser::Parser() {
  _input = "";
  _ast = NULL;
}

nts::Parser::~Parser() {}

void nts::Parser::feed(std::string const& input) {
  _input += input;
}

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
  // to do
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
  std::istringstream input(_input);
  std::string line;
  ASTSectionType currentSection = nts::ASTSectionType::UNDEFINED;
  std::smatch matched;
  std::regex regCom(REG_COM);
  std::regex regEmpty(REG_EMPTY);

  this->initTree();
  // loop file to store each line in `line` var and analyse it
  while (std::getline(input, line)) {
    if (!this->checkSection(currentSection, line) &&
        !std::regex_match(line, regCom) && !std::regex_match(line, regEmpty)) {
        this->checkWrongSection(line, currentSection);
        if (currentSection == nts::ASTSectionType::CHIPSET) { this->addChipset(line); }
        else { this->addLink(line); }
      }
    }
  return _ast;
}

void nts::Parser::setInputValues(const std::vector<std::pair<std::string, std::string>> &inputValues) {
  std::vector<std::pair<std::string, std::string>>::const_iterator itVal = inputValues.begin();

  std::vector<nts::IComponent *>::iterator itComp;
  return;
  while (itVal != inputValues.end()) {
    itComp = _comps.begin();
    while (itComp != _comps.end() && (*itComp)->getName() != (*itVal).first) {
      *itComp++;
    }
    //(*itComp)
    // thhrow if (itVal == inputValues.end())
    *itVal++;
  }
}

void nts::Parser::initComps(const int &size) {
  int i = 0;
  while (i < size) {
    _comps.push_back(NULL);
    i++;
  }
}

static nts::IComponent *transfComp(const nts::t_ast_node *comp) {
  std::smatch matched;
  std::regex regComp(comp->lexeme);
  std::string value;
  nts::IComponent *newNode;

  std::regex_search(comp->value.cbegin(), comp->value.cend(), matched, regComp);
  value = matched[2].str() + matched[3].str();
  newNode = nts::AComponent::createComponent(matched[1].str(), value);
  return newNode;
}

void nts::Parser::parseTree(t_ast_node& root) {
  return;
  this->initComps((int)(*root.children)[0]->children->size());
  std::transform((*root.children)[0]->children->begin(), (*root.children)[0]->children->end(), _comps.begin(), transfComp);
}
