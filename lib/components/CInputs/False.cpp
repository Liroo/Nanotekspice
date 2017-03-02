#include "False.hpp"

nts::False::False(const std::string &name) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::OUTPUT
    }),
    nts::Tristate::FALSE_NTS);
  _type = "false";
}
