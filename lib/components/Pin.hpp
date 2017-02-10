#ifndef PIN_HPP
# define PIN_HPP

# include <cstddef>
# include <utility>
# include <map>
# include "IComponent.hpp"

namespace nts {
  enum GateType {
    UNKNOWN = -1,
    NOR
  };
  class Pin;
  class FlowChart;
};

class nts::Pin {
  public:
    Pin(const nts::IComponent *comp = NULL);
    ~Pin();

  public:
    void setComp(const nts::IComponent *newComp);
    IComponent *getComp() const;
    void setState(const nts::Tristate &newState);
    Tristate getState() const;

  private:
    void setGate(const nts::FlowChart *);

  private:
    FlowChart *_gate;
    IComponent *_comp;
    Tristate _state;
};

typedef bool (*compPtr_t)(const nts::Pin *);

class nts::FlowChart {
  public:
    FlowChart(const std::pair<Pin *, Pin *> &inputs, const Pin &output, const nts::GateType &type);
    ~FlowChart() {};

  private:
    std::pair<Pin *, Pin *> _inputs;
    Pin *_output;
    GateType _type;

  private:
    std::map<GateType, compPtr_t> _fn;
};

#endif /* PIN_HPP */
