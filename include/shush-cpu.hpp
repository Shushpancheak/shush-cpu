#pragma once
#include "shush-stack.hpp"
#include "shush-dump.hpp"
#include "shush-logs.hpp"

namespace shush {
namespace cpu {

const inline std::string VERSION = "0.0.1";

enum Errc {
  ASSERTION_FAILED = -1,
  UNKNOWN_COMMAND  = 0
};

// In bytes, DRAM + SRAM + VRAM
const inline size_t MEMORY_SIZE = 1048576 * .5; // == .5 Mb
const inline size_t REGISTERS_COUNT = 32;
const inline size_t COMMANDS_COUNT  = 256;

const inline size_t CODE_SIZE   = MEMORY_SIZE / 2;
const inline size_t VARS_POS    = CODE_SIZE;
const inline size_t VARS_SIZE   = (MEMORY_SIZE * 3) / 10;
const inline size_t VRAM_POS    = VARS_SIZE + VARS_POS;
const inline size_t VRAM_SIZE   = MEMORY_SIZE / 5;

// in int64_t's
const inline size_t STACK_SIZE  = MEMORY_SIZE / 64; // == .5 Mb

/**
 * Current memory map:
 * [code] [vars] [VRAM]
 * [registers]
 * [stack]
 *
 * Memory map (future):
 * [code] [vars] [heap] [stack] [VRAM]
 * [registers]
 */
class Cpu {
public:
  void LoadCode(const char* const file_name);
  void Start(int argc, char** argv);
  void Run();

  inline void Push(int64_t val);
  inline void Pop (uint8_t reg);

  inline void Add ();
  inline void Sub ();
  inline void Div ();
  inline void Mod ();
  inline void Sqrt();

  inline void Goto(size_t pos);

  inline void In  ();
  inline void Out ();

  inline void End ();

  const char* GetDumpMessage(int error_code);


  char    mem[MEMORY_SIZE]     {};
  int64_t reg[REGISTERS_COUNT] {};
  stack::SafeStackStatic<int64_t, STACK_SIZE> stack;
};

}
}