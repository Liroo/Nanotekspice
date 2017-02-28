#include "True.hpp"

nts::True::True(const std::string &name) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::OUTPUT
    }), nts::Tristate::TRUE);
  _type = "true";
}
