#include "AComponent.hpp"

std::map<std::string, createFn_t> nts::AComponent::_fn = {
  { "4001", &nts::AComponent::create4001 },
  { "4008", &nts::AComponent::create4008 },
  { "4011", &nts::AComponent::create4011 },
  { "4013", &nts::AComponent::create4013 },
  { "4017", &nts::AComponent::create4017 },
  { "4030", &nts::AComponent::create4030 },
  { "4040", &nts::AComponent::create4040 },
  { "4069", &nts::AComponent::create4069 },
  { "4071", &nts::AComponent::create4071 },
  { "4081", &nts::AComponent::create4081 },
  { "4094", &nts::AComponent::create4094 },
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
                              const std::vector<nts::pinConf> &pinsConf,
                              const nts::Tristate &state)
{
  int i = 1;

  while (i <= size) {
    _pins[i] = new Pin(i, state);
    _pins[i]->setType(pinsConf[i]);
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
  });
}

int nts::AComponent::sizePins() const {
  return _realPins;
}

void nts::AComponent::SetLink(size_t pin_num_this,
                              nts::IComponent &component,
                              size_t pin_num_target) {

  std::function<void(nts::Pin *, nts::Pin *)> checkLink =
  [](nts::Pin *pin_this, nts::Pin *pin_target)->void {
    nts::pinConf typePin1, typePin2;

    //  Check if pins exists
    if (!pin_this || !pin_target) {
      throw nts::Exception::ComponentException(std::cerr, EPINNOEXISTS);
    }
    typePin1 = pin_this->getType();
    typePin2 = pin_target->getType();
    if ((typePin1 != nts::pinConf::FAKE && typePin2 != nts::pinConf::FAKE) &&
        ((typePin1 == nts::pinConf::INPUT && typePin2 != nts::pinConf::OUTPUT) ||
        (typePin1 == nts::pinConf::CLOCK && typePin2 != nts::pinConf::CLOCK) ||
        (typePin1 == nts::pinConf::OUTPUT && typePin2 != nts::pinConf::INPUT))) {
      throw nts::Exception::ComponentException(std::cerr, EPININVALIDTYPE);
    }
  };
  //  Check linkage error
  checkLink(_pins[pin_num_this], (component.getPins())[pin_num_target]);
  checkLink((component.getPins())[pin_num_target], _pins[pin_num_this]);
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

  std::cout << "[" << this->getType() << "\t" << _name << "]:\n";
  std::for_each(_pins.begin(), _pins.end(),
  [&x, this](const std::pair<int, nts::Pin *> &pair) {
    if (x >= _realPins) { return; }
    std::cout << "Pin n°" << pair.first << ": \t" << states[(pair.second)->getState()] << "\n";
    x++;
  });
}

nts::Tristate nts::AComponent::Compute(size_t pin_num_this) {

  // if pin is connected to an input-type (true, false, input, clock),
  // or if it has already been computed, return its state
  if (std::regex_match(this->getType(), std::regex(REG_INPUTTYPES)) ||
      _pins[pin_num_this]->getComputed() == nts::Tristate::TRUE) {
    _pins[pin_num_this]->setComputed(nts::Tristate::TRUE);
    return _pins[pin_num_this]->getState();
  }
  _pins[pin_num_this]->setComputed(nts::Tristate::TRUE);

  nts::FlowChart *gate = _pins[pin_num_this]->getGate();
  std::vector<nts::Pin *> *inputs = gate->getInputs();
  std::function<void (nts::Pin *)> computeInputPin =
    [](nts::Pin *pinInput) {
      nts::IComponent *linkedComp = pinInput->getLinkedComp();
      nts::Pin *linkedPin = pinInput->getLinkedPin();

      if (linkedComp) { pinInput->setState(linkedComp->Compute(linkedPin->getID())); }
    };

  //  if pin is not linked, just return undefined state
  if (!_pins[pin_num_this]->getLinkedPin()) {
    std::for_each(inputs->begin(), inputs->end(), computeInputPin);
    return nts::Tristate::UNDEFINED;
  }

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

nts::IComponent *nts::AComponent::create4017(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4017(value));
}

nts::IComponent *nts::AComponent::create4030(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4030(value));
}

nts::IComponent *nts::AComponent::create4040(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4040(value));
}

nts::IComponent *nts::AComponent::create4069(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4069(value));
}

nts::IComponent *nts::AComponent::create4071(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4071(value));
}

nts::IComponent *nts::AComponent::create4081(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4081(value));
}

nts::IComponent *nts::AComponent::create4094(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4094(value));
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
