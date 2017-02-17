#ifndef PIN_HPP
# define PIN_HPP

# include <cstddef>
# include <utility>
# include <map>
# include <regex>
# include "IComponent.hpp"

namespace nts {
  enum GateType {
    UNKNOWN = -1,
    NOR,
    NAND,
    OR,
    AND,
    XOR,
    FOURBITSADDER
  };
  class Pin;
  class FlowChart;
};

class nts::Pin {
  public:
    Pin(const nts::IComponent *,
        const int &id,
        const nts::Tristate &state = nts::Tristate::UNDEFINED,
        const nts::IComponent *comp = NULL,
        const nts::Tristate &computed = nts::Tristate::FALSE);
    ~Pin();

  public:
    void setComp(const nts::IComponent *newComp, const int &pin);
    IComponent *getLinkedComp() const;
    IComponent *getOwner() const;
    void setState(const nts::Tristate &newState);
    Tristate getState() const;
    int getID() const;
    nts::Pin *getLinkedPin() const;
    void setGate(const nts::FlowChart *);
    nts::FlowChart *getGate() const;
    void setComputed(const nts::Tristate &);
    nts::Tristate getComputed() const;

  private:
    FlowChart *_gate;
    IComponent *_linkedComp;
    IComponent *_owner;
    Pin *_linkedPin;
    Tristate _state;
    int _id;
    Tristate _computed;
};

typedef std::function<nts::Tristate(const nts::FlowChart *)> gateFn_t;


class nts::FlowChart {
  public:
    FlowChart(std::vector<Pin *> *inputs, std::vector<Pin *> *output, const nts::GateType &type);
    ~FlowChart() {};

  public:
    nts::Tristate Exec() const;
    bool hasDefinedPins() const;
    static nts::Tristate NOR(const nts::FlowChart *);
    static nts::Tristate OR(const nts::FlowChart *);
    static nts::Tristate AND(const nts::FlowChart *);
    static nts::Tristate NAND(const nts::FlowChart *);
    static nts::Tristate XOR(const nts::FlowChart *);
    static nts::Tristate fourBitsAdder(const nts::FlowChart *);
    static std::map<GateType, gateFn_t> _gateFn;

  public:
    std::vector<Pin *> *getOutputs() const;
    std::vector<Pin *> *getInputs() const;
    nts::GateType getType() const;

  private:
    std::vector<Pin *> *_inputs;
    std::vector<Pin *> *_outputs;
    GateType _type;
};

#endif /* PIN_HPP */
