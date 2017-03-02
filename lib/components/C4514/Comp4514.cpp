#include "Comp4514.hpp"

nts::C4514::C4514(const std::string &name) : AComponent(name, 52) {
  _type = "4514";
  this->initPins(52, std::vector<nts::pinConf>({
    nts::pinConf::NOLINK, // None
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::NOLINK, // Alim
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT, nts::pinConf::OUTPUT,
    nts::pinConf::INPUT, nts::pinConf::INPUT, nts::pinConf::INPUT,
    nts::pinConf::NOLINK, // Alim,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE,
    nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE, nts::pinConf::FAKE
  }));
  // set compute value to undefined for pin with alimentation purpose to ignore them later
  _pins[8]->setComputed(nts::Tristate::UNDEFINED);
  _pins[16]->setComputed(nts::Tristate::UNDEFINED);
  nts::IComponent *Ithis = reinterpret_cast<nts::IComponent *>(this);

  Ithis->SetLink(25, *Ithis, 29);
  Ithis->SetLink(26, *Ithis, 31);
  Ithis->SetLink(27, *Ithis, 33);
  Ithis->SetLink(28, *Ithis, 35);
  Ithis->SetLink(30, *Ithis, 37);
  Ithis->SetLink(32, *Ithis, 40);
  Ithis->SetLink(34, *Ithis, 43);
  Ithis->SetLink(36, *Ithis, 46);
  Ithis->SetLink(38, *Ithis, 49);
  Ithis->SetLink(41, *Ithis, 50);
  Ithis->SetLink(44, *Ithis, 51);
  Ithis->SetLink(47, *Ithis, 52);
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[2], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[25]}),
                                      nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[3], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[26]}),
                                      nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[21], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[27]}),
                                      nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[22], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[28]}),
                                      nts::GateType::NAND));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[29], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[30]}),
                                      nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[31], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[32]}),
                                      nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[33], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[34]}),
                                      nts::GateType::NAND));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[35], _pins[1]}),
                                      new std::vector<nts::Pin *>({_pins[36]}),
                                      nts::GateType::NAND));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[29], _pins[37]}),
                                      new std::vector<nts::Pin *>({_pins[38], _pins[39] }),
                                      nts::GateType::FLIPFLOPRS));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[31], _pins[40]}),
                                      new std::vector<nts::Pin *>({_pins[41], _pins[42]}),
                                      nts::GateType::FLIPFLOPRS));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[33], _pins[43]}),
                                      new std::vector<nts::Pin *>({_pins[44], _pins[45]}),
                                      nts::GateType::FLIPFLOPRS));
  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({_pins[35], _pins[46]}),
                                      new std::vector<nts::Pin *>({_pins[47], _pins[48]}),
                                      nts::GateType::FLIPFLOPRS));

  _gates.push_back(new nts::FlowChart(new std::vector<nts::Pin *>({ _pins[49], _pins[50], _pins[51], _pins[52], _pins[23]}),
                                      new std::vector<nts::Pin *>({ _pins[11], _pins[9], _pins[10], _pins[8],
                                                                    _pins[7], _pins[6], _pins[5], _pins[4],
                                                                    _pins[18], _pins[17], _pins[20], _pins[19],
                                                                    _pins[14], _pins[13], _pins[16], _pins[15]}),
                                      nts::GateType::DECODER));
}
