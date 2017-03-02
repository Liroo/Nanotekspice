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
    virtual bool isRising();
    virtual bool isFalling();

  private:
    nts::Tristate _mode;
};

#endif /* CLOCK_HPP */
