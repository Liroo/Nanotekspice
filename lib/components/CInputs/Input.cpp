#include "Input.hpp"

nts::Input::Input(const std::string &name) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::OUTPUT
    }));
  _type = "input";
}
