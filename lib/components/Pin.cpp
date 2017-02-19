#include "IComponent.hpp"

nts::Pin::Pin(const int &id,
              const nts::Tristate &state,
              const nts::IComponent *comp,
              const nts::Tristate &computed) {
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
  { nts::GateType::BITSADDER, &nts::FlowChart::bitsAdder },
  { nts::GateType::FLIPFLOP, &nts::FlowChart::flipFlop },
  { nts::GateType::NAND, &nts::FlowChart::NAND },
  { nts::GateType::AND, &nts::FlowChart::AND },
  { nts::GateType::OR, &nts::FlowChart::OR },
  { nts::GateType::XOR, &nts::FlowChart::XOR }
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

void nts::FlowChart::NOR(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  nts::Tristate newState;

  if (!gate->hasDefinedPins()) { newState = nts::Tristate::UNDEFINED; }
  else {
    newState =
      (nts::Tristate)std::accumulate(inputs->begin(), inputs->end(), 0,
      [](int &acc, nts::Pin *pin)->int {
        return acc ^ (int)pin->getState();
      });
    }
  std::for_each(outputs->begin(), outputs->end(),
  [&newState](nts::Pin *pinOutput) {
    pinOutput->setState(newState);
    pinOutput->getLinkedPin()->setState(newState);
  });
}

void nts::FlowChart::bitsAdder(const nts::FlowChart *gate) {
  std::vector<nts::Pin *> *outputs = gate->getOutputs();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  bool a = (*inputs)[0]->getState();
  bool b = (*inputs)[0]->getState();
  bool carryIn = (*inputs)[0]->getState();

  if (!gate->hasDefinedPins()) {
    (*outputs)[0]->setState(nts::Tristate::UNDEFINED);
    (*outputs)[1]->setState(nts::Tristate::UNDEFINED);
    return;
   }
  (*outputs)[0]->setState(nts::Tristate(a && b));
  (*outputs)[1]->setState(nts::Tristate((a && b) || (a && carryIn) || (b && carryIn)));
}

void nts::FlowChart::flipFlop(const nts::FlowChart *gate) {
  (void)gate;
}

void nts::FlowChart::NAND(const nts::FlowChart *gate) {
  (void)gate;
}

void nts::FlowChart::AND(const nts::FlowChart *gate) {
  (void)gate;
}

void nts::FlowChart::OR(const nts::FlowChart *gate) {
  (void)gate;
}

void nts::FlowChart::XOR(const nts::FlowChart *gate) {
  (void)gate;
}

void nts::FlowChart::Exec() const {
  _gateFn[_type](this);
}
