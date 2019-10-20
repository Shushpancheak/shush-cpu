#include "shush-cpu.hpp"


int main(int argc, char** argv) {
  shush::cpu::Cpu cpu;
  cpu.Start(argc, argv);
  return 0;
}


shush::cpu::Cpu::~Cpu() {
  delete[] mem;
}


void shush::cpu::Cpu::LoadCode(const char* const file_name) {
  FILE* file = fopen(file_name, "rb");

  if (!file) { // TODO automatize
    std::cerr << "Could not open file \"" << file_name << "\"" << std::endl;
    exit(Errc::COULD_NOT_OPEN_FILE);
  }

  struct stat file_stat {};
  fstat(_fileno(file), &file_stat);

  if (static_cast<size_t>(file_stat.st_size) > CODE_SIZE) {
    std::cerr << "File size is too big." << std::endl;
    exit(Errc::FILE_TOO_LARGE);
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
    exit(Errc::NO_FILE_GIVEN);
  }

  cpu.LoadCode(argv[1]);
  cpu.Run();
}


void shush::cpu::Cpu::Run() {
  // Following are just temporary variables
  char cur_byte = 0;
  double arg = 0;
  uint8_t reg = 0;
  size_t pos = 0;

  for (size_t i = 0; i < CODE_SIZE; ++i) {
    cur_byte = mem[i];

    switch (cur_byte) {
      case 0x00: {
        arg = *reinterpret_cast<double*>(mem + i + 1);
        i += sizeof(int64_t);
        Push(arg);
        break;
      }
      case 0x01: {
        reg = *reinterpret_cast<uint8_t*>(mem + i + 1);
        i += sizeof(uint8_t);
        Push(reg);
        break;
      }

      case 0x10: {
        Add();
        break;
      }
      case 0x11: {
        Sub();
        break;
      }
      case 0x12: {
        Div();
        break;
      }
      case 0x13: {
        Mod();
        break;
      }
      case 0x14: {
        Sqrt();
        break;
      }

      // goto
      case 0x20: {
        pos = *reinterpret_cast<size_t*>(mem + i + 1);
        i = pos; // TODO check for availability
        break;
      }

      // in
      case 0x30: {
        std::cin >> arg;
        Push(arg);
        break;
      }
      // out
      case 0x31: {
        Out();        
        break;
      }

      case 0x40: {
        End();
        break;
      }

      default: {
        std::cerr << "Unknown command" << std::endl;
        exit(Errc::UNKNOWN_COMMAND);
      }
    }
  } 
}


void shush::cpu::Cpu::Push(double val) {
  stack.Push(val);
}


void shush::cpu::Cpu::Pop(uint8_t reg_id) {
  reg[reg_id] = stack.Pop();
}


void shush::cpu::Cpu::Out() {
  std::cout << stack.Pop() << std::endl;
}


void shush::cpu::Cpu::Add() {
  stack.Push(stack.Pop() + stack.Pop());
}


void shush::cpu::Cpu::Sub() {
  stack.Push(stack.Pop() - stack.Pop());
}


void shush::cpu::Cpu::Div() {
  stack.Push(stack.Pop() / stack.Pop());
}


void shush::cpu::Cpu::Mod() {
  stack.Push(static_cast<int>(stack.Pop()) % 
             static_cast<int>(stack.Pop()));
}


void shush::cpu::Cpu::Sqrt() {
  stack.Push(sqrt(stack.Pop()));
}


void shush::cpu::Cpu::End() {
  std::cout << "Session ended." << std::endl;
  exit(0);
}


const char* shush::cpu::Cpu::GetDumpMessage(int error_code) {
  char* msg = new char[50]; // TODO automatize
  char err_name[50];
  switch (error_code) {
    case NO_ERROR : {
      memcpy(err_name, "No error", 9);
      break;
    }
    case ASSERTION_FAILED : {
      memcpy(err_name, "Assertion failed", 17);
      break;
    }
    case UNKNOWN_COMMAND : {
      memcpy(err_name, "An unknown command was read from memory", 40);
      break;
    }
    case NO_FILE_GIVEN : {
      memcpy(err_name, "No file was given to program", 29);
      break;
    }
    case COULD_NOT_OPEN_FILE : {
      memcpy(err_name, "Could not open the file given to program", 41);
      break;
    }
    case FILE_TOO_LARGE : {
      memcpy(err_name, "File size is too big", 21);
      break;
    }
    default : {
      memcpy(err_name, "UNKNOWN ERROR", 14);
      break;
    }
  }
  sprintf(msg, "CPU dump:\nError code: %d (%s)", error_code, err_name);
  return msg;
}
