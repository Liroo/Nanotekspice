#include "Comp4008.hpp"

nts::C4008::C4008(const std::string &name) : AComponent(name, 16) {
  _type = "4008";
  this->initPins(22, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK, // None
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK,
    nts::pinConf::INPUT,
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::INPUT,
    nts::pinConf::NOLINK
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[8]->setComputed(nts::Tristate::UNDEFINED);
  _pins[16]->setComputed(nts::Tristate::UNDEFINED);
  nts::IComponent *Ithis = reinterpret_cast<nts::IComponent *>(this);
  Ithis->SetLink(17, *Ithis, 18);
  Ithis->SetLink(19, *Ithis, 20);
  Ithis->SetLink(21, *Ithis, 22);

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[1], _pins[15], _pins[22]}),
                                      new std::vector<nts::Pin *>({_pins[13], _pins[14]}),
                                      nts::GateType::BITSADDER));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[2], _pins[3], _pins[20]}),
                                      new std::vector<nts::Pin *>({_pins[12], _pins[21]}),
                                      nts::GateType::BITSADDER));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[4], _pins[5], _pins[18]}),
                                      new std::vector<nts::Pin *>({_pins[11], _pins[19]}),
                                      nts::GateType::BITSADDER));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[6], _pins[7], _pins[9]}),
                                      new std::vector<nts::Pin *>({_pins[10], _pins[17]}),
                                      nts::GateType::BITSADDER));
}
