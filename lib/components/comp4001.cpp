#include "comp4001.hpp"

nts::C4001::C4001(const std::string &name) : AComponent(name) {
  _type = "4001";
  this->initPins(14);
  _gates[0] = new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[1], _pins[2]), *_pins[3], nts::GateType::NOR);
  _gates[1] = new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[5], _pins[6]), *_pins[4], nts::GateType::NOR);
  _gates[2] = new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[8], _pins[9]), *_pins[10], nts::GateType::NOR);
  _gates[3] = new nts::FlowChart(std::pair<nts::Pin *, nts::Pin *>(_pins[12], _pins[13]), *_pins[11], nts::GateType::NOR);
}
