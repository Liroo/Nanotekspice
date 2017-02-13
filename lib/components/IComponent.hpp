#ifndef ICOMPONENT_HPP
# define ICOMPONENT_HPP

namespace nts {
  enum Tristate {
    UNDEFINED = (-true),
    TRUE = true,
    FALSE = false
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


    virtual std::map<int, nts::Pin *> getPins() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getType() const = 0;
};

#endif /* ICOMPONENT_HPP */
