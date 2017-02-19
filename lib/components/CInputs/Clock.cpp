#include "Clock.hpp"

nts::Clock::Clock(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1, state);
  _type = "clock";
}
