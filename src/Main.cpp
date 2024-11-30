#include <iostream>
#include <stdexcept>

#include "../inc/Application.h"

int main() {
  try {
    Application app;
    app.run();
  } catch (std::exception& e) {
    std::cout << "\nEXCEPTION: " << e.what() << std::endl;
  }
}
