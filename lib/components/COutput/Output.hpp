#ifndef OUTPUT_HPP
# define OUTPUT_HPP

# include "AComponent.hpp"

namespace nts {
  class Output;
};

class nts::Output : public nts::AComponent {
  public:
    Output(const std::string &name);
    virtual ~Output() {};
};

#endif /* OUTPUT_HPP */
