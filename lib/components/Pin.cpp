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
  // TODO ignored pins ? VDD VSS
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
  { nts::GateType::XOR, &nts::FlowChart::XOR }
 };

nts::FlowChart::FlowChart(const std::pair<nts::Pin *, nts::Pin *> &inputs,
                          const nts::Pin &output,
                          const nts::GateType &type) {
  _inputs.push_back(inputs.first);
  _inputs.push_back(inputs.second);
  _output = const_cast<nts::Pin *>(&output);
  _type = type;
  _inputs[0]->setGate(this);
  _inputs[1]->setGate(this);
  _output->setGate(this);
}

std::vector<nts::Pin *> nts::FlowChart::getInputs() const {
  return _inputs;
}

nts::Pin *nts::FlowChart::getOutput() const {
  return _output;
}

nts::GateType nts::FlowChart::getType() const {
  return _type;
}

nts::Tristate nts::FlowChart::NOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> inputs = gate->getInputs();
  if (inputs[0]->getState() == nts::Tristate::UNDEFINED ||
      inputs[1]->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs[0]->getState() != nts::Tristate::TRUE || inputs[1]->getState() != nts::Tristate::TRUE);
}

nts::Tristate nts::FlowChart::NAND(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> inputs = gate->getInputs();
  if (inputs[0]->getState() == nts::Tristate::UNDEFINED ||
      inputs[1]->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs[0]->getState() != nts::Tristate::TRUE && inputs[1]->getState() != nts::Tristate::TRUE);
}

nts::Tristate nts::FlowChart::AND(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> inputs = gate->getInputs();
  if (inputs[0]->getState() == nts::Tristate::UNDEFINED ||
      inputs[1]->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs[0]->getState() == nts::Tristate::TRUE && inputs[1]->getState() == nts::Tristate::TRUE);
}

nts::Tristate nts::FlowChart::OR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> inputs = gate->getInputs();
  if (inputs[0]->getState() == nts::Tristate::UNDEFINED ||
      inputs[1]->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs[0]->getState() == nts::Tristate::TRUE || inputs[1]->getState() == nts::Tristate::TRUE);
}


nts::Tristate nts::FlowChart::XOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> inputs = gate->getInputs();
  if (inputs[0]->getState() == nts::Tristate::UNDEFINED ||
      inputs[1]->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)((inputs[0]->getState() == nts::Tristate::TRUE && inputs[1]->getState() != nts::Tristate::TRUE) ||
                          (inputs[0]->getState() != nts::Tristate::TRUE && inputs[1]->getState() == nts::Tristate::TRUE));
}

nts::Tristate nts::FlowChart::Exec() const {
  return _gateFn[_type](this);
}
