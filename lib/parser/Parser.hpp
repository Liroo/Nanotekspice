#ifndef PARSER_CPP
# define PARSER_CPP

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
# define REG_AVAILABLETYPE "input|output|4001"

namespace nts {
  class Parser {
    private:
      std::string _input;
      nts::t_ast_node* _ast;
      std::map<std::string, nts::IComponent *> _comps;
      bool _dirty;

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
                                  const std::string &value = "") const;
      void initComps(const int &);

    private:
      // check if the line is a new section, updates the current section if needed
      bool checkSection(nts::ASTSectionType &, const std::string &) const;
      // check if a line is in a section it shouldn't
      void checkWrongSection(const std::string &, const nts::ASTSectionType &) const;
      void addChipset(const std::string &);
      void addLink(const std::string &);

    public:
      std::map<std::string, nts::IComponent *> getComponentsMap() const;
      void setDirty(const bool &);
      bool isDirty() const;
  };
};

#endif
