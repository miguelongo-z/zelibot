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
    int allowed_user = std::stoi(getenv("ALLOWED_USER"));
    ZeliBot zelibot(token, allowed_user);
    zelibot.run();
  } catch (std::exception &e) {
    std::cout << "[ERROR]" << e.what() << std::endl;
  }
  return 0;
}
