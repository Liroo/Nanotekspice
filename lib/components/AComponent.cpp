#include "AComponent.hpp"

std::map<std::string, createFn_t> nts::AComponent::_fn = {
  { "4001", &nts::AComponent::create4001 },
  { "input", &nts::AComponent::createInput },
  { "output", &nts::AComponent::createOutput }
};

nts::AComponent::AComponent(const std::string &name) { _name = name; }

void nts::AComponent::initPins(const int &size, const nts::Tristate &state) {
  int i = 1;

  while (i <= size) {
    _pins[i] = new Pin(reinterpret_cast<nts::IComponent *>(this), i, state);
    i++;
  }
}

std::map<int, nts::Pin *> nts::AComponent::getPins() const {
  return _pins;
}

std::vector<nts::FlowChart *> nts::AComponent::getGates() const {
  return _gates;
}

std::string nts::AComponent::getName() const {
  return _name;
}

std::string nts::AComponent::getType() const {
  return _type;
}

void nts::AComponent::resetPins() const {
  std::for_each(_pins.begin(), _pins.end(),
  [](const std::pair<int, nts::Pin *> &pair) {
    if (pair.second->getComputed() != nts::Tristate::UNDEFINED) {
      (pair.second)->setComputed(nts::Tristate::FALSE);
    }
    nts::IComponent *linked = (pair.second)->getLinkedComp();
    if (linked && !std::regex_match(linked->getType(), std::regex(REG_INPUTTYPE))) {
      (pair.second)->setState(nts::Tristate::FALSE);
    }
  });
}

void nts::AComponent::SetLink(size_t pin_num_this,
                              nts::IComponent &component,
                              size_t pin_num_target) {
  // check logical links error, throw if needed TODO
  // maybe lexem in Pin class to check which types can be linked ?

  // links first component's pin to second's pin, and second way
  _pins[pin_num_this]->setComp(&component, pin_num_target);
  (component.getPins())[pin_num_target]->setComp(this, pin_num_this);
}

void nts::AComponent::Dump() const {
  static std::map<nts::Tristate, std::string> states = {
    { nts::Tristate::UNDEFINED, "undefined" },
    { nts::Tristate::FALSE, "false" },
    { nts::Tristate::TRUE, "true" }
  };

  std::cout << "[" << this->getType() << "\t" << _name << "]:" << std::endl;
  std::for_each(_pins.begin(), _pins.end(),
  [](const std::pair<int, nts::Pin *> &pair) {
    std::cout << "Pin n°" << pair.first << ": \t" << states[(pair.second)->getState()] << std::endl;
  });
}

static void computeInputPin(nts::Pin *pin) {
  nts::IComponent *linkedComp = pin->getLinkedComp();
  nts::Pin *linkedPin = pin->getLinkedPin();

  if (pin->getComputed() == nts::Tristate::TRUE) { return; }
  else if (linkedComp) { pin->setState(linkedComp->Compute(linkedPin->getID())); }
}

nts::Tristate nts::AComponent::Compute(size_t pin_num_this) {

  // if pin is connected to an input-type (true, false, input, clock),
  // or if it has already been computed, return its state
  if (std::regex_match(this->getType(), std::regex(REG_INPUTTYPE)) ||
      _pins[pin_num_this]->getComputed() == nts::Tristate::TRUE) {
    _pins[pin_num_this]->setComputed(nts::Tristate::TRUE);
    return _pins[pin_num_this]->getState();
  }

  _pins[pin_num_this]->setComputed(nts::Tristate::TRUE);
  //  if pin is not linked, just return undefined state
  if (!_pins[pin_num_this]->getLinkedPin()) {
    return nts::Tristate::UNDEFINED;
  }

  nts::FlowChart *gate = _pins[pin_num_this]->getGate();
  std::vector<nts::Pin *> inputs = gate->getInputs();
  nts::Pin *output = gate->getOutput();

  // if the computed pin is one of the gate's inputs, just compute its linked pin
  if (_pins[pin_num_this] == inputs[0] || _pins[pin_num_this] == inputs[1]) {
    computeInputPin(_pins[pin_num_this]);
  } else {
  // if the computed pin is the gate's output, compute both input then output
    nts::Tristate res;

    std::for_each(inputs.begin(), inputs.end(), computeInputPin);
    res = gate->Exec();
    output->setState(res);
    (output->getLinkedComp()->getPins()[1])->setState(res);
  }
  return _pins[pin_num_this]->getState();
}

nts::IComponent *nts::AComponent::create4001(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4001(value));
}

nts::IComponent *nts::AComponent::createInput(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::Input(value));
}

nts::IComponent *nts::AComponent::createOutput(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::Output(value));
}

nts::IComponent *nts::AComponent::createComponent(const std::string &type, const std::string &value){
  return _fn[type](value);
}
