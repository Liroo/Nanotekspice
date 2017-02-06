#ifndef REGEX_HPP
# define REGEX_HPP

# include "regex.h"
# include "BaseException.hpp"
# include <vector>

# define REGERROR_MAXSIZE 500

class Regex {
  // ctor && dtor
  public:
    Regex(const char *pattern);
    ~Regex();

    public:
      std::string exec(const char *string);
  private:
    regex_t _reg;
    regmatch_t _match;
};

#endif /* REGEX_HPP */
