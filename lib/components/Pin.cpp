#include "IComponent.hpp"

nts::Pin::Pin(const nts::IComponent *owner,
              const int &id,
              const nts::Tristate &state,
              const nts::IComponent *comp,
              const nts::Tristate &computed) {
  _owner = const_cast<nts::IComponent *>(owner);
  _id = id;
  _gate = NULL;
  _linkedComp = const_cast<IComponent *>(comp);
  _state = state;
  _linkedPin = NULL;
  _computed = computed;
}

void nts::Pin::setComp(const IComponent *newComp, const int &pin) {
  _linkedComp = const_cast<IComponent *>(newComp);
  _linkedPin = _linkedComp->getPins()[pin];
  _state = nts::Tristate::UNDEFINED;
}

nts::IComponent *nts::Pin::getLinkedComp() const {
  return _linkedComp;
}

nts::IComponent *nts::Pin::getOwner() const {
  return _owner;
}

void nts::Pin::setState(const nts::Tristate &newState) {
  _state = newState;
}

void nts::Pin::setComputed(const nts::Tristate &computed) {
  _computed = computed;
}

nts::Tristate nts::Pin::getComputed() const {
  return _computed;
}

nts::Tristate nts::Pin::getState() const {
  return _state;
}

int nts::Pin::getID() const {
  return _id;
}

nts::Pin *nts::Pin::getLinkedPin() const {
  return _linkedPin;
}

void nts::Pin::setGate(const nts::FlowChart *gate) {
  _gate = const_cast<nts::FlowChart *>(gate);
}

nts::FlowChart *nts::Pin::getGate() const {
  return _gate;
}

std::map<nts::GateType, gateFn_t> nts::FlowChart::_gateFn = {
  { nts::GateType::NOR, &nts::FlowChart::NOR },
  { nts::GateType::NAND, &nts::FlowChart::NAND },
  { nts::GateType::AND, &nts::FlowChart::AND },
  { nts::GateType::OR, &nts::FlowChart::OR },
  { nts::GateType::XOR, &nts::FlowChart::XOR },
  { nts::GateType::FOURBITSADDER, &nts::FlowChart::fourBitsAdder }
 };

nts::FlowChart::FlowChart(std::vector<nts::Pin *> *inputs,
                          std::vector<nts::Pin *> *outputs,
                          const nts::GateType &type) {
  std::function<void (nts::Pin *)> assignGate =
  [this](nts::Pin *pin)->void {
    pin->setGate(this);
  };

  _inputs = inputs;
  _outputs = outputs;
  std::for_each(_inputs->begin(), _inputs->end(), assignGate);
  std::for_each(_outputs->begin(), _outputs->end(), assignGate);
  _type = type;
}

std::vector<nts::Pin *> *nts::FlowChart::getInputs() const {
  return _inputs;
}

std::vector<nts::Pin *> *nts::FlowChart::getOutputs() const {
  return _outputs;
}

nts::GateType nts::FlowChart::getType() const {
  return _type;
}

bool nts::FlowChart::hasDefinedPins() const {
  return std::find_if(_inputs->begin(), _inputs->end(),
          [](const nts::Pin *pin) {
            return pin->getState() == nts::Tristate::UNDEFINED;
          }) == _inputs->end();
}

nts::Tristate nts::FlowChart::NOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *inputs = gate->getInputs();

  if (!gate->hasDefinedPins()) { return nts::Tristate::UNDEFINED; }
  return nts::Tristate(std::accumulate(inputs->begin(), inputs->end(), 0,
          [](int &acc, nts::Pin *pin)->int {
            return acc ^ (int)pin->getState();
          }));
}

nts::Tristate nts::FlowChart::NAND(const nts::FlowChart *gate) {
  (void)gate;
  if (!gate->hasDefinedPins()) { return nts::Tristate::UNDEFINED; }
  return nts::Tristate::TRUE;
}

nts::Tristate nts::FlowChart::AND(const nts::FlowChart *gate) {
  (void)gate;
  return nts::Tristate::UNDEFINED;
}

nts::Tristate nts::FlowChart::OR(const nts::FlowChart *gate) {
  (void)gate;
  return nts::Tristate::UNDEFINED;
}

nts::Tristate nts::FlowChart::XOR(const nts::FlowChart *gate) {
  (void)gate;
  return nts::Tristate::UNDEFINED;
}

nts::Tristate nts::FlowChart::fourBitsAdder(const nts::FlowChart *gate) {
  (void)gate;
  return nts::Tristate::UNDEFINED;
}

nts::Tristate nts::FlowChart::Exec() const {
  return _gateFn[_type](this);
}
