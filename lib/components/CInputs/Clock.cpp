#include "Clock.hpp"

nts::Clock::Clock(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1, state);
  _type = "clock";
  _rising = false;
}

bool nts::Clock::isRising() const {
  return _rising == true;
}

void nts::Clock::uploadRising() {
  _rising = _pins[1]->getState() == nts::Tristate::TRUE;
}
