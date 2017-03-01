#include "Input.hpp"

nts::Input::Input(const std::string &name, const nts::Tristate &state) : AComponent(name, 1) {
  this->initPins(1,
    std::vector<nts::pinConf>({
      nts::pinConf::NOLINK, // None
      nts::pinConf::OUTPUT
    }),
    state);
  _type = "input";
}
