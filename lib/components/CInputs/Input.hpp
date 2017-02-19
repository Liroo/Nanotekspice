#ifndef INPUT_HPP
# define INPUT_HPP

# include "AComponent.hpp"

namespace nts {
  class Input;
};

class nts::Input : public nts::AComponent {
  public:
    Input(const std::string &name, const nts::Tristate &state = nts::Tristate::UNDEFINED);
    virtual ~Input() {};
};

#endif /* INPUT_HPP */
