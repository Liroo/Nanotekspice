#include "Clock.hpp"

nts::Clock::Clock(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::CLOCK
    }),
    state);
  _type = "clock";
  _rising = false;
}

bool nts::Clock::isRising() const {
  return _rising == true;
}

void nts::Clock::uploadRising(const bool &value) {
  _rising = value == nts::Tristate::TRUE;
}
