#ifndef ACOMPONENT_HPP
# define ACOMPONENT_HPP

namespace nts {
  class AComponent;
};

# include <functional>
# include "IComponent.hpp"
# include "ParserException.hpp"

typedef std::function<nts::IComponent *(const std::string &)> createFn_t;

# define REG_INPUTTYPE "^(?:input|clock|true|false)$"

class nts::AComponent : public nts::IComponent {
  public:
    AComponent(const std::string &, const int &);
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
    virtual void resetPins() const;

  public:
    static IComponent *createComponent(const std::string &type, const std::string &value);
    static IComponent *create4001(const std::string &value);
    static IComponent *create4008(const std::string &value);
    static IComponent *create4011(const std::string &value);
    static IComponent *create4013(const std::string &value);
    static IComponent *create4017(const std::string &value);
    static IComponent *create4030(const std::string &value);
    static IComponent *create4040(const std::string &value);
    static IComponent *create4069(const std::string &value);
    static IComponent *create4071(const std::string &value);
    static IComponent *create4081(const std::string &value);
    static IComponent *create4094(const std::string &value);
    static IComponent *createInput(const std::string &value);
    static IComponent *createClock(const std::string &value);
    static IComponent *createTrue(const std::string &value);
    static IComponent *createFalse(const std::string &value);
    static IComponent *createOutput(const std::string &value);
    static std::map<std::string, createFn_t> _fn;

    public:
      virtual bool isRising() const { return false; };
      virtual void uploadRising() {};

  protected:
    std::string _name;
    std::string _type;
    std::vector<nts::FlowChart *> _gates;
    std::map<int, nts::Pin *> _pins;
    const int _realPins;
};

# include "Comp4001.hpp"
# include "Comp4008.hpp"
# include "Comp4011.hpp"
# include "Comp4013.hpp"
# include "Comp4017.hpp"
# include "Comp4030.hpp"
# include "Comp4040.hpp"
# include "Comp4069.hpp"
# include "Comp4071.hpp"
# include "Comp4081.hpp"
# include "Comp4094.hpp"
# include "Input.hpp"
# include "Clock.hpp"
# include "True.hpp"
# include "False.hpp"
# include "Output.hpp"

#endif /* ACOMPONENT_HPP */
