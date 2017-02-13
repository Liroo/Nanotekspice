#include "AComponent.hpp"

std::map<std::string, createFn_t> nts::AComponent::_fn = { { "4001", &nts::AComponent::create4001 }, { "input", &nts::AComponent::createInput } };

nts::AComponent::AComponent(const std::string &name, const nts::Tristate &value) { _name = name; _value = value; }

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

std::string nts::AComponent::getName() const {
  return _name;
}

std::string nts::AComponent::getType() const {
  return _type;
}

void nts::AComponent::setValue(const nts::Tristate &state) {
  _value = state;
}

void nts::AComponent::SetLink(size_t pin_num_this,
                              nts::IComponent &component,
                              size_t pin_num_target) {
  // check logical links error
  _pins[pin_num_this]->setComp(&component, pin_num_target);
  (component.getPins())[pin_num_target]->setComp(this, pin_num_this);
}

static void dumpName(const std::pair<int, nts::Pin *>pin) {
  if (pin.second->getLinkedComp()->getType() == "input") {
    std::cout << pin.second->getState();
  }
  else { std::cout << "not an input"; }
  std::cout << std::endl;
}

void nts::AComponent::Dump() const {
  std::cout << _name << ": ";
  std::for_each(_pins.begin(), _pins.end(), dumpName);
}

nts::IComponent *nts::AComponent::create4001(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::C4001(value));
}

nts::IComponent *nts::AComponent::createInput(const std::string &value) {
  return reinterpret_cast<nts::IComponent *>(new nts::Input(value));
}

nts::IComponent *nts::AComponent::createComponent(const std::string &type, const std::string &value){
  return _fn[type](value);
}

nts::Tristate nts::AComponent::Compute(size_t pin_num_this) {
  // throw if invalid pin num

  // if pin is connected to an input-type (true, false, input, clock), return its state
  if (std::regex_match(this->getType(), std::regex(REG_INPUTTYPE))) {
    return _pins[pin_num_this]->getState();
  }

  nts::FlowChart *gate = _pins[pin_num_this]->getGate();
  std::pair<nts::Pin *, nts::Pin *> inputs = gate->getInputs();
  nts::Pin *output = gate->getOutput();

  if (_pins[pin_num_this] == inputs.first || _pins[pin_num_this] == inputs.second) {
    nts::Pin *linked = _pins[pin_num_this]->getLinkedPin();
    _pins[pin_num_this]->setState(linked->getOwner()->Compute(linked->getID()));
  } else {
    // compute both input then output
    (inputs.first)->setState((inputs.first)->getLinkedComp()->Compute((inputs.first)->getLinkedPin()->getID()));
    (inputs.second)->setState((inputs.second)->getLinkedComp()->Compute((inputs.second)->getLinkedPin()->getID()));
    output->setState(gate->Exec());
  }
  return _pins[pin_num_this]->getState();
}
