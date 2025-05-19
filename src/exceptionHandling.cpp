#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>


const char* logFilePath = "./LOG";


namespace openApp {
  void exceptionsStart() {
    struct stat b;
    if (stat(logFilePath, &b) == 0)
      std::remove(logFilePath);
  }

  void logError(std::string type, std::string function, std::string variable, std::string what) {

    std::ofstream stream(logFilePath, std::ios::binary | std::ios::app);

    stream << type << "---" << "Function : " << function << " ::: At : " << std::chrono::system_clock::now() << " ::: Log : " << what << "\n";

    stream.flush();
    stream.close();
  }
}