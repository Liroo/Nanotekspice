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

  std::regex_search(line, matched, regSect);
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
  // to do, throw if needed
}

void nts::Parser::addChipset(const std::string &line) {
  std::regex regSpeChipset(REG_SPECHIPSET);
  std::regex regChipset(REG_CHIPSET);
  std::string lexem;

  if (std::regex_match(line, regSpeChipset)) {
    lexem = REG_SPECHIPSET;
  } else if (regex_match(line, regChipset)) {
    lexem = REG_CHIPSET;
  } else {
    std::cout << "bad format chipset line: " << line << std::endl;
    // throw
    return;
  }
  (*_ast->children)[0]->children->push_back(this->createNode(lexem, nts::ASTNodeType::COMPONENT, line));
}

void nts::Parser::addLink(const std::string &line) {
  std::smatch matched;
  std::regex regLinks(REG_LINKS);

  // throw if bad syntax
  std::regex_search(line, matched, regLinks);
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
  std::cout << "------ create tree --------" << std::endl;
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

void nts::Parser::setInputValues(const std::vector<std::pair<std::string, std::string>> &inputValues) {
  std::for_each(inputValues.begin(), inputValues.end(),
  [this](const std::pair<std::string, std::string> &inputValue) {
    std::map<std::string, nts::IComponent *>::iterator it = _comps.find(inputValue.first);
    int value;

    if (it != _comps.end()) {
      std::stringstream(inputValue.second) >> value;
      _comps[inputValue.first]->setValue((nts::Tristate)value);
    }
    else {
      std::cout << "not found: " << inputValue.first << std::endl;
      // throw ?
    }
  });
}

void nts::Parser::parseTree(t_ast_node& root) {
  // transform old tree's component branch to new components map
  std::for_each((*root.children)[0]->children->begin(), (*root.children)[0]->children->end(),
  [this](const t_ast_node *nodeComp) {
    std::smatch matched;
    std::regex regComp(nodeComp->lexeme);
    nts::IComponent *newNode;

    std::regex_search(nodeComp->value, matched, regComp);
    newNode = nts::AComponent::createComponent(matched[1].str(), matched[2].str());
    _comps.insert(std::make_pair(matched[2].str(), newNode));
    //_comps[matched[1].str()].insert(std::make_pair(matched[2].str(), newNode));
  });

  // links the component's pins using the IComponent's method SetLink
  unsigned int i = 0;
  std::smatch matched;
  std::smatch matchedSecond;
  std::regex regLink(REG_LINK);
  size_t firstPin;
  size_t secondPin;

  while (i < (*root.children)[1]->children->size()) {
    std::regex_search((*(*root.children)[1]->children)[i]->value, matched, regLink);
    std::regex_search((*(*root.children)[2]->children)[i]->value, matchedSecond, regLink);
    std::stringstream(matched[2].str()) >> firstPin;
    std::stringstream(matchedSecond[2].str()) >> secondPin;
    _comps[matched[1].str()]->SetLink(firstPin, *_comps[matchedSecond[1].str()], secondPin);
    i++;
  }
}
