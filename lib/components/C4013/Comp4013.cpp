#include "Comp4013.hpp"

nts::C4013::C4013(const std::string &name) : AComponent(name, 14) {
  _type = "4013";
  this->initPins(16, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK, // None
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::CLOCK, nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK, // Alim
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::CLOCK, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::NOLINK, // Alim,
    nts::pinConf::FAKE, nts::pinConf::FAKE
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[7]->setComputed(nts::Tristate::UNDEFINED);
  _pins[14]->setComputed(nts::Tristate::UNDEFINED);
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[3], _pins[4], _pins[5], _pins[6]}),
                                      new std::vector<nts::Pin *>({_pins[1], _pins[2], _pins[15]}),
                                      nts::GateType::FLIPFLOP));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[11], _pins[10], _pins[9], _pins[8]}),
                                      new std::vector<nts::Pin *>({_pins[13], _pins[12], _pins[16]}),
                                      nts::GateType::FLIPFLOP));
}
