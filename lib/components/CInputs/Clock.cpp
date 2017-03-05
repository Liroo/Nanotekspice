#include "Clock.hpp"

nts::Clock::Clock(const std::string &name) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::CLOCK
    }));
  _type = "clock";
}

bool nts::Clock::isRising() {
  return _pins[1]->getState() == nts::Tristate::TRUE;
}

bool nts::Clock::isFalling() {
  return _pins[1]->getState() == nts::Tristate::FALSE;
}
