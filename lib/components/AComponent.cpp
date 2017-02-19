#include "AComponent.hpp"

std::map<std::string, createFn_t> nts::AComponent::_fn = {
  { "4001", &nts::AComponent::create4001 },
  { "4008", &nts::AComponent::create4008 },
  { "input", &nts::AComponent::createInput },
  { "clock", &nts::AComponent::createClock },
  { "true", &nts::AComponent::createTrue },
  { "false", &nts::AComponent::createFalse },
  { "output", &nts::AComponent::createOutput }
};

nts::AComponent::AComponent(const std::string &name, const int &realPins) : _realPins(realPins) {
  _name = name;
}

void nts::AComponent::initPins(const int &size,
                              const nts::Tristate &state) {
  int i = 1;

  while (i <= size) {
    _pins[i] = new Pin(i, state);
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
    /* TODO Check if needed
    nts::IComponent *linked = (pair.second)->getLinkedComp();
    if (linked && !std::regex_match(linked->getType(), std::regex(REG_INPUTTYPE))) {
      (pair.second)->setState(nts::Tristate::FALSE);
    } */
  });
}

void nts::AComponent::SetLink(size_t pin_num_this,
                              nts::IComponent &component,
                              size_t pin_num_target) {
  // check logical links error, throw if needed TODO
  // maybe lexem in Pin class to check which types can be linked ?

  // invalid pin
  // variable output on pin input
  // variable input on pin output
  // pin input - pin input || pin output - pin output

  // links first component's pin to second one's pin, and second way
  _pins[pin_num_this]->setComp(&component, pin_num_target);
  (component.getPins())[pin_num_target]->setComp(this, pin_num_this);
}

void nts::AComponent::Dump() const {
  static std::map<nts::Tristate, std::string> states = {
    { nts::Tristate::UNDEFINED, "undefined" },
    { nts::Tristate::FALSE, "false" },
    { nts::Tristate::TRUE, "true" }
  };
  int x = 0;

  std::cout << "[" << this->getType() << "\t" << _name << "]:" << std::endl;
  std::for_each(_pins.begin(), _pins.end(),
  [&x, this](const std::pair<int, nts::Pin *> &pair) {
    if (x >= _realPins) { return; }
    std::cout << "Pin n°" << pair.first << ": \t" << states[(pair.second)->getState()] << std::endl;
    x++;
  });
}

nts::Tristate nts::AComponent::Compute(size_t pin_num_this) {

  std::cout << "compute: " << this->getName() << ": pin: " << pin_num_this << std::endl;

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
  std::vector<nts::Pin *> *inputs = gate->getInputs();

  std::function<void (nts::Pin *)> computeInputPin =
    [](nts::Pin *pinInput) {
      nts::IComponent *linkedComp = pinInput->getLinkedComp();
      nts::Pin *linkedPin = pinInput->getLinkedPin();
      if (linkedComp) { pinInput->setState(linkedComp->Compute(linkedPin->getID())); }
    };

  // if the computed pin is one of the gate's inputs, just compute its linked pin
  if (std::find(inputs->begin(), inputs->end(), _pins[pin_num_this]) != inputs->end()) {
    computeInputPin(_pins[pin_num_this]);
  } else {
  // if the computed pin is the gate's output, compute both input then exec the gate's associated function
    std::for_each(inputs->begin(), inputs->end(), computeInputPin);
    gate->Exec();
  }
  return _pins[pin_num_this]->getState();
}

nts::IComponent *nts::AComponent::create4001(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4001(value));
}

nts::IComponent *nts::AComponent::create4008(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4008(value));
}

nts::IComponent *nts::AComponent::create4011(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4011(value));
}

nts::IComponent *nts::AComponent::create4013(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4013(value));
}

nts::IComponent *nts::AComponent::createInput(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::Input(value));
}

nts::IComponent *nts::AComponent::createClock(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::Clock(value));
}

nts::IComponent *nts::AComponent::createTrue(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::True(value));
}

nts::IComponent *nts::AComponent::createFalse(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::False(value));
}

nts::IComponent *nts::AComponent::createOutput(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::Output(value));
}

nts::IComponent *nts::AComponent::createComponent(const std::string &type, const std::string &value){
  return _fn[type](value);
}
