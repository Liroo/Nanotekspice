#include "Comp4030.hpp"

nts::C4030::C4030(const std::string &name) : AComponent(name, 14) {
  _type = "4030";
  this->initPins(14, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK, // None
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::OUTPUT,
    nts::pinConf::OUTPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK, // Alim
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::OUTPUT,
    nts::pinConf::OUTPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK // Alim
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[7]->setComputed(nts::Tristate::UNDEFINED);
  _pins[14]->setComputed(nts::Tristate::UNDEFINED);
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[1], _pins[2]}),
                                      new std::vector<nts::Pin *>({_pins[3]}),
                                      nts::GateType::XOR));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[5], _pins[6]}),
                                      new std::vector<nts::Pin *>({_pins[4]}),
                                      nts::GateType::XOR));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[8], _pins[9]}),
                                      new std::vector<nts::Pin *>({_pins[10]}),
                                      nts::GateType::XOR));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[12], _pins[13]}),
                                      new std::vector<nts::Pin *>({_pins[11]}),
                                      nts::GateType::XOR));
}
