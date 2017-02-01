#include "BaseException.hpp"

nts::Exception::BaseException::BaseException(std::ostream &os, const std::string &msg)
  : _os(os) {
  _msg = msg;
}

nts::Exception::BaseException::~BaseException() throw() {}

const char* nts::Exception::BaseException::what() const throw() {
  return _msg.c_str();
}

std::ostream& nts::Exception::BaseException::getOs() const {
  return _os;
}
