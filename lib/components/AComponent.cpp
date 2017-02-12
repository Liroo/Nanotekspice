#include "AComponent.hpp"

/*static nts::IComponent *create4001(const std::string &value);
static nts::IComponent *createInput(const std::string &value);
*/
std::map<std::string, creatPtr_t> nts::AComponent::_fn = { { "4001", &nts::AComponent::create4001 }, { "input", &nts::AComponent::createInput } };

nts::AComponent::AComponent(const std::string &name) { _name = name; }

void nts::AComponent::initPins(const int &size, const nts::Tristate &state) {
  int i = 1;

  while (i <= size) {
    _pins[i] = new Pin(state);
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

void nts::AComponent::SetLink(size_t pin_num_this,
                              nts::IComponent &component,
                              size_t pin_num_target) {
  _pins[pin_num_this]->setComp((component.getPins())[pin_num_target]->getComp());
}

void dumpName(const std::pair<int, nts::Pin *>pin) {
  if (pin.second->getComp()->getType() == "input") {
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
  return reinterpret_cast<nts::IComponent *>(new nts::Input(value, nts::Tristate::TRUE));
}

nts::IComponent *nts::AComponent::createComponent(const std::string &type, const std::string &value){
  return _fn[type](value);
}

nts::Tristate nts::AComponent::Compute(size_t pin_num_this) {
  (void)pin_num_this;
  return nts::Tristate::UNDEFINED;
}
