#ifndef ICOMPONENT_HPP
# define ICOMPONENT_HPP

namespace nts {
  enum Tristate {
    UNDEFINED = (-true),
    TRUE_NTS = true,
    FALSE_NTS = false
  };
  enum pinConf {
    NOLINK = -1,
    INPUT,
    CLOCK,
    OUTPUT,
    FAKE
  };
  class IComponent;
};

# include "Pin.hpp"
# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <algorithm>

class nts::IComponent {
  public:
    virtual ~IComponent(void) { };
    /// Compute value of the precised pin
    virtual nts::Tristate Compute(size_t pin_num_this = 1) = 0;
    /// Useful to link IComponent together
    virtual void SetLink(size_t pin_num_this,
                          nts::IComponent &component,
                          size_t pin_num_target) = 0;
    ///// Print on terminal the name of the component and
    //// the state of every pin of the current component
    virtual void Dump(void) const = 0;


  public:
    virtual std::map<int, nts::Pin *> getPins() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getType() const = 0;
    virtual std::vector<nts::FlowChart *> getGates() const = 0;

  public:
    virtual bool isRising() const  = 0;
    virtual bool isFalling() const  = 0;
    virtual void uploadMode(const bool &) = 0;

  public:
    virtual void resetPins() const = 0;
    virtual int sizePins() const = 0;
};

#endif /* ICOMPONENT_HPP */
