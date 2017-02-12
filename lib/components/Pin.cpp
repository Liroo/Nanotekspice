#include "IComponent.hpp"

nts::Pin::Pin(const nts::Tristate &state, const nts::IComponent *comp) {
  _gate = NULL;
  _comp = const_cast<IComponent *>(comp);
  _state = state;
}

void nts::Pin::setComp(const IComponent *newComp) {
  _comp = const_cast<IComponent *>(newComp);
}

nts::IComponent *nts::Pin::getComp() const {
  return _comp;
}

void nts::Pin::setState(const nts::Tristate &newState) {
  _state = newState;
}

nts::Tristate nts::Pin::getState() const {
  return _state;
}

void nts::Pin::setGate(const nts::FlowChart *gate) {
  _gate = const_cast<nts::FlowChart *>(gate);
}

std::map<nts::GateType, compPtr_t> _fn = { { nts::GateType::NOR, &nts::FlowChart::NOR } };

nts::FlowChart::FlowChart(const std::pair<nts::Pin *, nts::Pin *> &inputs,
                          const nts::Pin &output,
                          const nts::GateType &type) {
  _inputs = inputs;
  _output = const_cast<nts::Pin *>(&output);
  _type = type;
}

bool nts::FlowChart::NOR(const nts::Pin *pin) {
  (void)pin;
  return true;
}
