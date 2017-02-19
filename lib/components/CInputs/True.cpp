#include "True.hpp"

nts::True::True(const std::string &name) : AComponent(name, 1) {
  this->initPins(1, nts::Tristate::TRUE);
  _type = "true";
}
