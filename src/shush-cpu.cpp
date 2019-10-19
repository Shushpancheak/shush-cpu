#include "shush-cpu.hpp"


int main(int argc, char** argv) {
  shush::cpu::Cpu cpu;
  std::cout << "shush::cpu emulator greets you! The version you running: "
            << shush::cpu::VERSION << std::endl;
}