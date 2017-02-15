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
    FlowChart(const std::pair<Pin *, Pin *> &inputs, const Pin &output, const nts::GateType &type);
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
    static std::map<std::string, std::string> tables;

  public:
    nts::Pin *getOutput() const;
    nts::GateType getType() const;
    std::vector<Pin *> getInputs() const;

  private:
    std::vector<Pin *> _inputs;
    Pin *_output;
    GateType _type;
    struct _config {
      std::regex lexem;
      nts::Tristate value;;
    };
    std::vector<_config> _table;
};

#endif /* PIN_HPP */
