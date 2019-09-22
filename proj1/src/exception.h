#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <string>

class exception: public std::exception {
public:

  exception() {
    error_code_ = 0;
  }

  explicit exception(std::string message) {
    error_message_ = message;
    error_code_ = 0;
  }

  explicit exception(std::string message, long code) {
    error_message_ = message;
    error_code_ = code;
  }

  virtual const char* what() const throw () {
    return error_message_.c_str();
  }

  virtual const std::string &message() {
    return error_message_;
  }

  virtual const long code() const throw () {
    return error_code_;
  }

  virtual ~exception() throw () {}

private:
  std::string error_message_;
  long error_code_;

};

#endif