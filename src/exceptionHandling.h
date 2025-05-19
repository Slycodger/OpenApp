#pragma once
#include <string>

namespace openApp {
  void exceptionsStart();
  void logError(std::string type, std::string function, std::string variable, std::string what);
}