#include "Comp4011.hpp"

nts::C4011::C4011(const std::string &name) : AComponent(name) {
  _type = "4011";
  this->initPins(14);
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[7]->setComputed(nts::Tristate::UNDEFINED);
  _pins[14]->setComputed(nts::Tristate::UNDEFINED);
  _gates.push_back(new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[1], _pins[2]), *_pins[3], nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[5], _pins[6]), *_pins[4], nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[8], _pins[9]), *_pins[10], nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[12], _pins[13]), *_pins[11], nts::GateType::NAND));
}
