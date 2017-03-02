#include "Comp2716.hpp"

nts::C2716::C2716(const std::string &name) : AComponent(name, 24) {
  _type = "2716";
  this->initPins(24, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK // None
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[12]->setComputed(nts::Tristate::UNDEFINED);
  _pins[24]->setComputed(nts::Tristate::UNDEFINED);
}
