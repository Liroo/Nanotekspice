#include "Input.hpp"

nts::Input::Input(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1, state);
  _type = "input";
}
