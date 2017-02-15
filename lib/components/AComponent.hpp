#ifndef ACOMPONENT_HPP
# define ACOMPONENT_HPP

namespace nts {
  class AComponent;
};

# include <functional>
# include "IComponent.hpp"
# include "BaseException.hpp"

typedef std::function<nts::IComponent *(const std::string &)> createFn_t;

# define REG_INPUTTYPE "^(?:input|clock|true|false)$"

class nts::AComponent : public nts::IComponent {
  public:
    AComponent(const std::string &);
    virtual ~AComponent() {};

  public:
    virtual nts::Tristate Compute(size_t pin_num_this = 1);
    virtual void SetLink(size_t pin_num_this,
                          nts::IComponent &component,
                          size_t pin_num_target);
    virtual void Dump(void) const;

  public:
    virtual std::string getName() const;
    virtual std::string getType() const;
    virtual std::map<int, nts::Pin *> getPins() const;
    virtual std::vector<nts::FlowChart *> getGates() const;

  protected:
    void initPins(const int &, const Tristate &state = Tristate::UNDEFINED);
  public:
    void resetPins() const;

  public:
    static IComponent *createComponent(const std::string &type, const std::string &value);
    static IComponent *create4001(const std::string &value);
    static IComponent *createInput(const std::string &value);
    static IComponent *createOutput(const std::string &value);
    static std::map<std::string, createFn_t> _fn;

  protected:
    std::string _name;
    std::string _type;
    std::vector<nts::FlowChart *> _gates;
    std::map<int, nts::Pin *> _pins;
};

# include "Comp4001.hpp"
# include "Input.hpp"
# include "Output.hpp"

#endif /* ACOMPONENT_HPP */
