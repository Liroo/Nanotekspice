#ifndef ACOMPONENT_HPP
# define ACOMPONENT_HPP

namespace nts {
  class AComponent;
};

# include "IComponent.hpp"

typedef nts::IComponent *(*creatPtr_t)(const std::string &value);

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

  protected:
    void initPins(const int &, const Tristate &state = Tristate::UNDEFINED);

  public:
    static IComponent *createComponent(const std::string &type, const std::string &value);
    static IComponent *create4001(const std::string &value);
    static IComponent *createInput(const std::string &value);
  protected:
    static std::map<std::string, creatPtr_t> _fn;

  protected:
    std::string _name;
    std::string _type;
    std::vector<nts::FlowChart *> _gates;
    std::map<int, nts::Pin *> _pins;
};

# include "comp4001.hpp"
# include "Input.hpp"

#endif /* ACOMPONENT_HPP */
