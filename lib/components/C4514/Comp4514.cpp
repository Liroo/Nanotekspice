#include "Comp4514.hpp"

nts::C4514::C4514(const std::string &name) : AComponent(name, 24) {
  _type = "4514";
  this->initPins(28, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK, // None
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::NOLINK, // Alim
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK, // Alim,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[8]->setComputed(nts::Tristate::UNDEFINED);
  _pins[16]->setComputed(nts::Tristate::UNDEFINED);
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[1], _pins[2], _pins[3], _pins[21], _pins[22]}),
                                      new std::vector<nts::Pin *>({_pins[25], _pins[26], _pins[27], _pins[28]}),
                                      nts::GateType::FLIPFLOP));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({ _pins[25], _pins[26], _pins[27], _pins[28]}),
                                      new std::vector<nts::Pin *>({ _pins[11], _pins[9], _pins[10], _pins[8],
                                                                    _pins[7], _pins[6], _pins[5], _pins[4],
                                                                    _pins[18], _pins[17], _pins[20], _pins[19],
                                                                    _pins[14], _pins[13], _pins[16], _pins[15]}),
                                      nts::GateType::DECODER));
}
