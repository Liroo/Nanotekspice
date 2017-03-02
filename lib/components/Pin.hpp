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
    BITSADDER,
    FLIPFLOPD,
    FLIPFLOPRS,
    TENBITSJOHNSONDECADE,
    TWELVEBITSCOUNTER,
    NOT,
    EIGHTBITSSHIFTER,
    DECODER
  };
  class Pin;
  class FlowChart;
};

class nts::Pin {
  public:
    Pin(const int &id = -1,
        const nts::Tristate &state = nts::Tristate::UNDEFINED,
        const nts::IComponent *comp = NULL,
        const nts::Tristate &computed = nts::Tristate::FALSE);
    ~Pin();

  public:
    void setComp(const nts::IComponent *newComp, const int &pin);
    IComponent *getLinkedComp() const;
    void setState(const nts::Tristate &newState);
    void updateState(const nts::Tristate &newState);
    Tristate getState() const;
    int getID() const;
    nts::Pin *getLinkedPin() const;
    void setGate(const nts::FlowChart *);
    nts::FlowChart *getGate() const;
    void setComputed(const nts::Tristate &);
    nts::Tristate getComputed() const;
    void setType(const nts::pinConf &);
    nts::pinConf getType() const;

  private:
    FlowChart *_gate;
    IComponent *_linkedComp;
    Pin *_linkedPin;
    Tristate _state;
    int _id;
    Tristate _computed;
    nts::pinConf _type;
};

typedef std::function<void(const nts::FlowChart *)> gateFn_t;

class nts::FlowChart {
  public:
    FlowChart(std::vector<Pin *> *inputs, std::vector<Pin *> *output, const nts::GateType &type);
    ~FlowChart() {};

  public:
    void Exec() const;
    bool hasDefinedPins() const;
    static void NOR(const nts::FlowChart *);
    static void OR(const nts::FlowChart *);
    static void AND(const nts::FlowChart *);
    static void NAND(const nts::FlowChart *);
    static void XOR(const nts::FlowChart *);
    static void bitsAdder(const nts::FlowChart *);
    static void flipFlopD(const nts::FlowChart *);
    static void flipFlopRS(const nts::FlowChart *);
    static void tenBitsJohnsonDecade(const nts::FlowChart *gate);
    static void twelveBitsCounter(const nts::FlowChart *gate);
    static void NOT(const nts::FlowChart *gate);
    static void eightBitsShifter(const nts::FlowChart *gate);
    static void decoder(const nts::FlowChart *gate);
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
