#include "Comp4094.hpp"

nts::C4094::C4094(const std::string &name) : AComponent(name, 16) {
  _type = "4094";
  this->initPins(16);
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[8]->setComputed(nts::Tristate::UNDEFINED);
  _pins[16]->setComputed(nts::Tristate::UNDEFINED);
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[1], _pins[2], _pins[3], _pins[15]}),
                                      new std::vector<nts::Pin *>({_pins[4], _pins[5], _pins[6], _pins[7], _pins[14], _pins[13], _pins[12], _pins[11], _pins[9], _pins[8]}),
                                      nts::GateType::EIGHTBITSSHIFTER));
}
