#include "includes/zelibot.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

int main() {
  try {
    std::string token(getenv("TOKEN"));
    printf("[ZeliBOT] Token: %s\n", token.c_str());
    ZeliBot zelibot(token);
    zelibot.run();
  } catch (std::exception &e) {
    std::cout << "[ERROR]" << e.what() << std::endl;
  }
  return 0;
}
