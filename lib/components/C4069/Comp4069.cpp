#include "Comp4069.hpp"

nts::C4069::C4069(const std::string &name) : AComponent(name, 14) {
  _type = "4069";
  this->initPins(14, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK, // None
    nts::pinConf::INPUT, nts::pinConf::OUTPUT,
    nts::pinConf::INPUT, nts::pinConf::OUTPUT,
    nts::pinConf::INPUT, nts::pinConf::OUTPUT,
    nts::pinConf::NOLINK,
    nts::pinConf::OUTPUT, nts::pinConf::INPUT,
    nts::pinConf::OUTPUT, nts::pinConf::INPUT,
    nts::pinConf::OUTPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK // Alim
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[7]->setComputed(nts::Tristate::UNDEFINED);
  _pins[14]->setComputed(nts::Tristate::UNDEFINED);
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[2]}),
                                      nts::GateType::NOT));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[3]}),
                                      new std::vector<nts::Pin *>({_pins[4]}),
                                      nts::GateType::NOT));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[5]}),
                                      new std::vector<nts::Pin *>({_pins[6]}),
                                      nts::GateType::NOT));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[9]}),
                                      new std::vector<nts::Pin *>({_pins[8]}),
                                      nts::GateType::NOT));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[11]}),
                                      new std::vector<nts::Pin *>({_pins[10]}),
                                      nts::GateType::NOT));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[13]}),
                                      new std::vector<nts::Pin *>({_pins[12]}),
                                      nts::GateType::NOT));
}
