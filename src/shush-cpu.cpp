#include "shush-cpu.hpp"


int main(int argc, char** argv) {
  shush::cpu::Cpu cpu;
  cpu.Start(argc, argv);
  return 0;
}


void shush::cpu::Cpu::LoadCode(const char* const file_name) {
  FILE* file = fopen(file_name, "rb");

  if (!file) {
    std::cerr << "Could not open file \"" << file_name << "\"" << std::endl;
    exit(1);
  }

  struct stat file_stat;
  fstat(_fileno(file), &file_stat);

  if (file_stat.st_size > CODE_SIZE) {
    std::cerr << "File is too big." << std::endl;
    exit(1);
  }

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
  cpu.Run();
}


void shush::cpu::Cpu::Run() {
  char cur_byte = 0;
  int64_t arg = 0;
  uint8_t reg = 0;

  for (size_t i = 0; i < CODE_SIZE; ++i) {
    cur_byte = mem[i];

    switch (cur_byte) {
      case 0x00: {
        arg = *reinterpret_cast<int64_t*>(mem + i + 1);
        i += sizeof(int64_t);
        Push(arg);
        break;
      }
      case 0x01: {
        reg = *reinterpret_cast<uint8_t*>(mem + i + 1);
        i += sizeof(uint8_t);
        Push(arg);
      }

      case 0x02: {
        reg = *reinterpret_cast<uint8_t*>(mem + i + 1);
        i += sizeof(uint8_t);
        Push(arg);
      }
    }
  } 
}
