#include "shush-cpu.hpp"


int main(int argc, char** argv) {
  try {
    setbuf(stdout, nullptr);
    shush::cpu::Cpu cpu;
    cpu.Start(argc, argv);
  } catch (shush::dump::Dump& dump) {
    shush::dump::HandleFinalDump(dump);
  }
  return 0;
}


shush::cpu::Cpu::~Cpu() {
  delete[] mem;
}


void shush::cpu::Cpu::LoadCode(const char* const file_name) {
  shush::file::File file(file_name, "rb");

  const size_t file_size = file.GetFileSize();

  UMASSERT(file_size <= CODE_SIZE, FILE_TOO_LARGE);

  file.Read(mem, file_size);
}


void shush::cpu::Cpu::Start(int argc, char** argv) {
  shush::cpu::Cpu cpu;
  std::cout << "shush::cpu emulator greets you! The version you're running: "
            << shush::cpu::VERSION << std::endl;

  UMASSERT(argc > 1, NO_FILE_GIVEN);

  cpu.LoadCode(argv[1]);
  cpu.Run();
}


void shush::cpu::Cpu::Goto(size_t& i) {
  size_t pos;
  pos = *reinterpret_cast<size_t*>(mem + i + 1);
  // Decreasing by one because of `for` loop
  i   = pos - 1; // TODO check for availability
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

      #include "commands.inc"

      default: {
        UMASSERT(true, UNKNOWN_COMMAND);
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
  std::cout << stack.GetElement(stack.GetCurSize() - 1) << std::endl;
}


void shush::cpu::Cpu::Add() {
  stack.Push(stack.Pop() + stack.Pop());
}


void shush::cpu::Cpu::Sub() {
  stack.Push(stack.Pop() - stack.Pop());
}


void shush::cpu::Cpu::Mul() {
  stack.Push(stack.Pop() * stack.Pop());
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
  return;
}


const char* shush::cpu::Cpu::GetDumpMessage(int error_code) {
  sprintf(dump_msg_buffer, "CPU dump:\nError code: %d (%s)", error_code, dump_error_name_buffer);
  return dump_msg_buffer;
}


const char* shush::cpu::Cpu::GetErrorName(int error_code) {
  switch (error_code) {
    case UNKNOWN_COMMAND : {
      strcpy(dump_error_name_buffer, "An unknown command was read from memory");
      break;
    }
    case NO_FILE_GIVEN : {
      strcpy(dump_error_name_buffer, "No file was given to program");
      break;
    }
    case COULD_NOT_OPEN_FILE : {
      strcpy(dump_error_name_buffer, "Could not open the file given to program");
      break;
    }
    case FILE_TOO_LARGE : {
      strcpy(dump_error_name_buffer, "File size is too big");
      break;
    }
    default : {
      strcpy(dump_error_name_buffer, "UNKNOWN ERROR");
      break;
    }
  }
  return dump_error_name_buffer;
}
