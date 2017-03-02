#ifndef PARSER_CPP
# define PARSER_CPP

namespace nts {
  class Parser;
};

# include "IParser.hpp"
# include "AComponent.hpp"
# include <iostream>
# include <sstream>
# include <fstream>
# include <utility>
# include <regex>

# define REG_SECTION "^.((?:links|chipsets)):$"
# define REG_COM "^#.*$"
# define REG_EMPTY "^$"
# define REG_SPECHIPSET "^(2716)\\s+(\\w+\\(\\w+\\))$"
# define REG_CHIPSET "^(\\w+)\\s+(\\w+)$"
# define REG_LINKS "^(\\w+:\\d+)\\s+(\\w+:\\d+)$"
# define REG_LINK "^(\\w+):(\\d+)$"
# define REG_AVAILABLETYPE "input|clock|true|false|output|4001|4008|4011|4013|4017|4030|4040|4069|4071|4081|4094|4514"

namespace nts {
  class Parser {
    private:
      std::string _input;
      nts::t_ast_node* _ast;
      std::map<std::string, nts::IComponent *> _comps;

    public:
      Parser();
      virtual ~Parser();

    public:
      virtual void feed(std::string const& input);
      virtual void parseTree(t_ast_node& root);
      virtual t_ast_node *createTree();
      void setInputValues(const std::vector<std::pair<std::string, std::string>> &);

    private:
      void initTree();
      nts::t_ast_node *createNode(const std::string &lexeme,
                                  const nts::ASTNodeType &type,
                                  const std::string &value = "",
                                  const unsigned int line = -1) const;
      void initComps(const int &);

    private:
      bool checkString(const std::string &, const unsigned int);
      bool checkNewLine(const std::string &, const unsigned int);
      // check if the line is a new section, updates the current section if needed
      bool checkSection(nts::ASTSectionType &, const std::string &, const unsigned int) const;
      // check if a line is in a section it shouldn't
      void checkWrongSection(const std::string &, const nts::ASTSectionType &) const;
      void addChipset(const std::string &, const unsigned int);
      void addLink(const std::string &, const unsigned int);

    public:
      std::map<std::string, nts::IComponent *> getComponentsMap() const;

  };
};

#endif
