#include "IComponent.hpp"

nts::Pin::Pin(const nts::IComponent *owner ,const int &id, const nts::Tristate &state, const nts::IComponent *comp) {
  _owner = const_cast<nts::IComponent *>(owner);
  _id = id;
  _gate = NULL;
  _linkedComp = const_cast<IComponent *>(comp);
  _state = state;
  _linkedPin = NULL;
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
  _inputs = inputs;
  _output = const_cast<nts::Pin *>(&output);
  _type = type;
  inputs.first->setGate(this);
  inputs.second->setGate(this);
  _output->setGate(this);
}

std::pair<nts::Pin *, nts::Pin *> nts::FlowChart::getInputs() const {
  return _inputs;
}

nts::Pin *nts::FlowChart::getOutput() const {
  return _output;
}

nts::GateType nts::FlowChart::getType() const {
  return _type;
}

nts::Tristate nts::FlowChart::NOR(const nts::FlowChart *gate) {
  std::pair<nts::Pin *, nts::Pin *> inputs = gate->getInputs();
  if (inputs.first->getState() == nts::Tristate::UNDEFINED ||
      inputs.second->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs.first->getState() != nts::Tristate::TRUE || inputs.second->getState() != nts::Tristate::TRUE);
}

nts::Tristate nts::FlowChart::NAND(const nts::FlowChart *gate) {
  std::pair<nts::Pin *, nts::Pin *> inputs = gate->getInputs();
  if (inputs.first->getState() == nts::Tristate::UNDEFINED ||
      inputs.second->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs.first->getState() != nts::Tristate::TRUE && inputs.second->getState() != nts::Tristate::TRUE);
}

nts::Tristate nts::FlowChart::AND(const nts::FlowChart *gate) {
  std::pair<nts::Pin *, nts::Pin *> inputs = gate->getInputs();
  if (inputs.first->getState() == nts::Tristate::UNDEFINED ||
      inputs.second->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs.first->getState() == nts::Tristate::TRUE && inputs.second->getState() == nts::Tristate::TRUE);
}

nts::Tristate nts::FlowChart::OR(const nts::FlowChart *gate) {
  std::pair<nts::Pin *, nts::Pin *> inputs = gate->getInputs();
  if (inputs.first->getState() == nts::Tristate::UNDEFINED ||
      inputs.second->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)(inputs.first->getState() == nts::Tristate::TRUE || inputs.second->getState() == nts::Tristate::TRUE);
}


nts::Tristate nts::FlowChart::XOR(const nts::FlowChart *gate) {
  std::pair<nts::Pin *, nts::Pin *> inputs = gate->getInputs();
  if (inputs.first->getState() == nts::Tristate::UNDEFINED ||
      inputs.second->getState() == nts::Tristate::UNDEFINED) {
        return nts::Tristate::UNDEFINED;
      }
  return (nts::Tristate)((inputs.first->getState() == nts::Tristate::TRUE && inputs.second->getState() != nts::Tristate::TRUE) ||
                          (inputs.first->getState() != nts::Tristate::TRUE && inputs.second->getState() == nts::Tristate::TRUE));
}

nts::Tristate nts::FlowChart::Exec() const {
  return _gateFn[_type](this);
}
