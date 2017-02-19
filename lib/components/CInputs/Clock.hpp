#ifndef CLOCK_HPP
# define CLOCK_HPP

# include "AComponent.hpp"

namespace nts {
  class Clock;
};

class nts::Clock : public nts::AComponent {
  public:
    Clock(const std::string &name, const nts::Tristate &state = nts::Tristate::UNDEFINED);
    virtual ~Clock() {};
};

#endif /* CLOCK_HPP */
