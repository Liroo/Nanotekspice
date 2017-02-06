#include "Regex.hpp"

Regex::Regex(const char *pattern) {
  int err = regcomp(&_reg, pattern, REG_EXTENDED);
  char err_msg[REGERROR_MAXSIZE + 1];

  if (err) {
    regerror(err, &_reg, (char *)err_msg, REGERROR_MAXSIZE + 1);
    throw nts::Exception::BaseException(std::cerr, err_msg);
  }
}

Regex::~Regex() {
  regfree(&_reg);
}

std::string Regex::exec(const char *string){
  regmatch_t match;
  std::string matched;

  if (regexec(&_reg, string, 1, &match, 0) == REG_NOMATCH) {
    return "";
  }
  return matched.assign(string, match.rm_so, match.rm_eo - match.rm_so);
}
