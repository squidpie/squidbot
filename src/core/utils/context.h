#pragma once
#include <string>

class Context {
public:
  Context(std::string lib_dir) : lib_dir(lib_dir) {}
  virtual ~Context() {}
  std::string lib_dir;
};