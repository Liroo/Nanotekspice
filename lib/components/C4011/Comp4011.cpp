#include "Comp4011.hpp"

nts::C4011::C4011(const std::string &name) : AComponent(name, 14) {
  _type = "4011";
  this->initPins(14);
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[7]->setComputed(nts::Tristate::UNDEFINED);
  _pins[14]->setComputed(nts::Tristate::UNDEFINED);

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[1], _pins[2]}),
                                      new std::vector<nts::Pin *>({_pins[3]}),
                                      nts::GateType::NAND));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[5], _pins[6]}),
                                      new std::vector<nts::Pin *>({_pins[4]}),
                                      nts::GateType::NAND));


_gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[8], _pins[9]}),
                                      new std::vector<nts::Pin *>({_pins[10]}),
                                      nts::GateType::NAND));


_gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[12], _pins[13]}),
                                    new std::vector<nts::Pin *>({_pins[11]}),
                                    nts::GateType::NAND));
}
