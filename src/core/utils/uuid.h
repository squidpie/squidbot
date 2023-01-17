#pragma once

#include <iostream>

typedef std::size_t uuid_t;

static inline uuid_t uuid(std::string input) {
  return std::hash<std::string>{}(input);
}