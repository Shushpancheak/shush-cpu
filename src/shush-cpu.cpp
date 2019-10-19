#include "shush-cpu.hpp"


int main(int argc, char** argv) {
  shush::cpu::Cpu cpu;
  cpu.Start(argc, argv);
  return 0;
}


void shush::cpu::Cpu::LoadCode(const char* const file_name) {
  FILE* file;
  (file = fopen(file_name, "rb")) ASSERTED;

  if (!file) {
    std::cerr << "Could not open file \"" << file_name << "\"" << std::endl;
    exit(1);
  }

  struct stat file_stat;
  fstat(_fileno(file), &file_stat);

  fread(mem, sizeof(char), file_stat.st_size, file);

  fclose(file);
}


void shush::cpu::Cpu::Start(int argc, char** argv) {
  shush::cpu::Cpu cpu;
  std::cout << "shush::cpu emulator greets you! The version you're running: "
            << shush::cpu::VERSION << std::endl;

  if (argc == 1) {
    std::cerr << "No file name given -- terminating.";
    exit(1);
  }

  cpu.LoadCode(argv[1]);
}
