#include "Input.hpp"

nts::Input::Input(const std::string &name, const nts::Tristate &state) : AComponent(name) {
  _state = state;
  _type = "input";
}

nts::Tristate nts::Input::getState() const {
  return _state;
}
