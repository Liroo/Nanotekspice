#include "Parser.hpp"

nts::Parser::Parser() {
  // init parser
  _input = "";
  _ast = NULL;
}

nts::Parser::~Parser() {}

void nts::Parser::feed(std::string const& input) {
  // concat input variable from class with input from arg
  _input += input;
}

nts::t_ast_node *nts::Parser::createNode(const std::string &lexeme,
                                          const nts::ASTNodeType &type,
                                          const std::string &value,
                                          const unsigned int line) const {
  nts::t_ast_node *newNode = new nts::t_ast_node(new std::vector<nts::t_ast_node *>);
  newNode->lexeme = lexeme;
  newNode->type = type;
  newNode->value = value;
  newNode->children = new std::vector<struct s_ast_node*>;
  newNode->line = line;
  return newNode;
}

void nts::Parser::initTree() {
  _ast = this->createNode("root", nts::ASTNodeType::DEFAULT);
  _ast->children->push_back(this->createNode("components list", nts::ASTNodeType::COMPONENT));
  _ast->children->push_back(this->createNode("link list", nts::ASTNodeType::LINK));
  _ast->children->push_back(this->createNode("link_end list", nts::ASTNodeType::LINK_END));
  _ast->children->push_back(this->createNode("section list", nts::ASTNodeType::SECTION));
  _ast->children->push_back(this->createNode("string list", nts::ASTNodeType::STRING));
  _ast->children->push_back(this->createNode("newline list", nts::ASTNodeType::NEWLINE));
}

bool nts::Parser::checkSection(nts::ASTSectionType &currentSection, const std::string &line, const unsigned int lineCount) const {
  std::smatch matched;
  std::regex regSect(REG_SECTION);

  std::regex_search(line, matched, regSect);
  if (matched.size() > 0) {
    if (matched[1] == "links") {
      currentSection = nts::ASTSectionType::LINK;
    }
    else if (matched[1] == "chipsets") {
      currentSection = nts::ASTSectionType::CHIPSET;
    }
    else {
      currentSection = nts::ASTSectionType::UNDEFINED;
      throw nts::Exception::ParserException(std::cerr, line + ": " + EPARSINVALIDSECTION);
    }
    (*_ast->children)[3]->children->push_back(this->createNode("#", nts::ASTNodeType::SECTION, matched[1], lineCount));
    return true;
  }
  return false;
}

void nts::Parser::checkWrongSection(const std::string &line, const nts::ASTSectionType &currentSection) const {
  if ((currentSection == nts::ASTSectionType::CHIPSET &&
      !std::regex_match(line, std::regex(REG_CHIPSET)) && !std::regex_match(line, std::regex(REG_SPECHIPSET))) ||
      ((currentSection == nts::ASTSectionType::LINK && !std::regex_match(line, std::regex(REG_LINKS))))) {
    throw nts::Exception::ParserException(std::cerr, line + ": " + EPARSBADSYNTAX);
  }
}

bool nts::Parser::checkString(const std::string &line, const unsigned int lineCount) {
  std::smatch matched;
  std::regex regCom(REG_COM);
  if (!std::regex_match(line, regCom)) { return false; }
  (*_ast->children)[4]->children->push_back(this->createNode("", nts::ASTNodeType::NEWLINE, line, lineCount));
  return true;
}

bool nts::Parser::checkNewLine(const std::string &line, const unsigned int lineCount) {
  std::smatch matched;
  std::regex regEmpty(REG_EMPTY);
  if (!std::regex_match(line, regEmpty)) { return false; }
  (*_ast->children)[5]->children->push_back(this->createNode("", nts::ASTNodeType::STRING, "", lineCount));
  return true;
}

void nts::Parser::addChipset(const std::string &line, const unsigned int lineCount) {
  std::string lexem;
  std::smatch matched;


  if (std::regex_match(line, std::regex(REG_SPECHIPSET))) { lexem = REG_SPECHIPSET; }
  else if (std::regex_match(line, std::regex(REG_CHIPSET))) { lexem = REG_CHIPSET; }
  else { throw nts::Exception::ParserException(std::cerr, line + ": " + EPARSBADSYNTAX); }
  std::regex_search(line, matched, std::regex(lexem));

  //  check if type is available
  if (!std::regex_match(matched[1].str(), std::regex(REG_AVAILABLETYPE))) {
    throw nts::Exception::ParserException(std::cerr, matched[1].str() + EPARSINVALIDTYPE);
  }

  // check if this chipset's name already exists
  if (std::find_if((*_ast->children)[0]->children->begin(), (*_ast->children)[0]->children->end(),
      [&matched](const nts::t_ast_node *comp)->bool {
        std::smatch matchedComp;

        std::regex_search(comp->value, matchedComp, std::regex(comp->lexeme));
        return matchedComp[2].str() == matched[2].str();
      }) != (*_ast->children)[0]->children->end()) {
        throw nts::Exception::ParserException(std::cerr, matched[2].str() + ": " + EPARSARGEXISTS);
      }
  (*_ast->children)[0]->children->push_back(this->createNode(lexem, nts::ASTNodeType::COMPONENT, line, lineCount));
}

void nts::Parser::addLink(const std::string &line, const unsigned int lineCount) {
  std::smatch matched;
  std::regex regLinks(REG_LINKS);

  if (!std::regex_match(line, regLinks)) { throw nts::Exception::ParserException(std::cerr, line + ": " + EPARSBADSYNTAX); }
  std::regex_search(line, matched, regLinks);
  (*_ast->children)[1]->children->push_back(this->createNode(REG_LINK, nts::ASTNodeType::LINK, matched[1], lineCount));
  (*_ast->children)[2]->children->push_back(this->createNode(REG_LINK, nts::ASTNodeType::LINK_END, matched[2], lineCount));
}

nts::t_ast_node* nts::Parser::createTree() {
  std::istringstream input(_input);
  std::string line;
  ASTSectionType currentSection = nts::ASTSectionType::UNDEFINED;
  std::smatch matched;
  std::regex regCom(REG_COM);
  std::regex regEmpty(REG_EMPTY);
  unsigned int lineCount = 0;

  this->initTree();
  //  loop file to store each line in `line` var and analyse it
  while (std::getline(input, line)) {
    //  check if section is valid (chipsets or links)
    if (!this->checkSection(currentSection, line, lineCount) &&
        // ignore comments and empty lines
        !checkString(line, lineCount) && !checkNewLine(line, lineCount)) {
      //  check if line is bad formated according to section
      this->checkWrongSection(line, currentSection);
      //  add chipset / link to the associated list
      if (currentSection == nts::ASTSectionType::CHIPSET) { this->addChipset(line, lineCount); }
      else { this->addLink(line, lineCount); }
    }
    lineCount += 1;
  }
  //  check if there are components and links
  if ((*_ast->children)[0]->children->size() == 0) { throw nts::Exception::ParserException(std::cerr, EPARSMISSINGCHIPSETS); }
  else if ((*_ast->children)[1]->children->size() == 0) { throw nts::Exception::ParserException(std::cerr, EPARSMISSINGLINKS); }

  return _ast;
}

void nts::Parser::setInputValues(const std::vector<std::pair<std::string, std::string>> &inputValues) {

  //  loop for every inputeValue, check if it's a real input variable and set its value
  std::for_each(inputValues.begin(), inputValues.end(),
  [this](const std::pair<std::string, std::string> &inputValue) {
    std::map<std::string, nts::IComponent *>::iterator it = _comps.find(inputValue.first);
    int value;

    if (it != _comps.end()) {
      //  check if variable initialized in args is an input
      if ((*it).second->getType() != "input" && (*it).second->getType() != "clock") {
        throw nts::Exception::ParserException(std::cerr, (*it).second->getType() + " " + inputValue.first + ": " + EPARSARGBADTYPE);
      }
      std::stringstream(inputValue.second) >> value;
      _comps[inputValue.first]->getPins()[1]->setState((nts::Tristate)value);
    }
    else {
      //  variable initialized but not defined in the config file
      throw nts::Exception::ParserException(std::cerr, inputValue.first + ": " + EPARSARGNOTFOUND);
    }
  });

  //  check if an input variable is not initialized
  std::map<std::string, nts::IComponent *>::iterator itPair;

  if ((itPair = std::find_if(_comps.begin(), _comps.end(),
      [this](const std::pair<std::string, nts::IComponent *> &pair)->bool {
        return ((pair.second)->getType() == "input" || (pair.second)->getType() == "clock") &&
                (pair.second->getPins()[1])->getState() == nts::Tristate::UNDEFINED;
      })) != _comps.end()) {
      throw nts::Exception::ParserException(std::cerr, (*itPair).first + ": " + EPARSARGNOTSET);
    }
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
    //  check if chipsets to link are defined
    if ((std::find_if(_comps.begin(), _comps.end(),
        [&matched](const std::pair<std::string, nts::IComponent*> &pair){
          return (pair.second)->getName() == matched[1].str();
        })) == _comps.end()) {
          throw nts::Exception::ParserException(std::cerr, EPARSINVALIDLINK + matched[1].str());
        }
    if ((std::find_if(_comps.begin(), _comps.end(),
        [&matchedSecond](const std::pair<std::string, nts::IComponent*> &pair){
          return (pair.second)->getName() == matchedSecond[1].str();
        })) == _comps.end()) {
          throw nts::Exception::ParserException(std::cerr, EPARSINVALIDLINK + matchedSecond[1].str());
        }
    //  links chipsets
    _comps[matched[1].str()]->SetLink(firstPin, *_comps[matchedSecond[1].str()], secondPin);
    i++;
  }

  //  check if every output is linked
  std::for_each(_comps.begin(), _comps.end(),
  [](const std::pair<std::string, nts::IComponent*> &pair) {
    if ((pair.second)->getType() == "output" && !((pair.second)->getPins()[1]->getLinkedComp())) {
      throw nts::Exception::ParserException(std::cerr, (pair.second)->getName() + EPARSOUTPUTNOTLINKED);
    }
  });

}

std::map<std::string, nts::IComponent *> nts::Parser::getComponentsMap() const {
  return _comps;
}
