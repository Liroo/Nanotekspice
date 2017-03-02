#include "Clock.hpp"

nts::Clock::Clock(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::CLOCK
    }),
    state);
  _type = "clock";
  _mode = nts::Tristate::UNDEFINED;
}

bool nts::Clock::isRising() const {
  return _mode == nts::Tristate::TRUE_NTS;
}

bool nts::Clock::isFalling() const {
  return _mode == nts::Tristate::FALSE_NTS;
}

void nts::Clock::uploadMode(const bool &value) {
  _mode = (nts::Tristate)value;
}
