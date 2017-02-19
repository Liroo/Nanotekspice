#include "False.hpp"

nts::False::False(const std::string &name) : AComponent(name, 1) {
  this->initPins(1, nts::Tristate::FALSE);
  _type = "false";
}
