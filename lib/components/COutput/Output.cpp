#include "Output.hpp"

nts::Output::Output(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1, state);
  _type = "output";
}
