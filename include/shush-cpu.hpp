#pragma once
#include "shush-stack.hpp"
#include "shush-dump.hpp"
#include "shush-logs.hpp"

namespace shush {
namespace cpu {

const inline std::string VERSION = "0.0.1";

enum Errc {
  NO_ERROR            = -2,
  ASSERTION_FAILED    = -1,
  UNKNOWN_COMMAND     = 0,
  NO_FILE_GIVEN       = 1,
  COULD_NOT_OPEN_FILE = 2,
  FILE_TOO_LARGE      = 3
};

// In bytes, DRAM + SRAM + VRAM
const inline size_t MEMORY_SIZE = 1048576 * 1; // == 1 Mb
const inline size_t REGISTERS_COUNT = 32;

const inline size_t CODE_SIZE   = MEMORY_SIZE / 2;
const inline size_t VARS_POS    = CODE_SIZE;
const inline size_t VARS_SIZE   = (MEMORY_SIZE * 3) / 10;
const inline size_t VRAM_POS    = VARS_SIZE + VARS_POS;
const inline size_t VRAM_SIZE   = MEMORY_SIZE / 5;

// in int64_t's
const inline size_t STACK_SIZE  = MEMORY_SIZE / 64; // == 1 Mb

/**
 * Loads code in memory and executes it.
 *
 * All operations are performed mostly on stack. The stack holds double values.
 * Arithmetic operations are done using first popped element as first operand
 * and the second popped as second operand.
 *
 * Current memory map:
 * [code] [vars] [VRAM]
 * [registers]
 * [stack]
 *
 * Memory map (future):
 * [code] [vars] [heap] [stack] [VRAM]
 * [registers]
 *
 */
class Cpu {
public:
  ~Cpu();

  /**
   * Load code in memory. Fails if the file is bad in some way.
   */
  void LoadCode(const char* const file_name);
  /**
   * Does everything to start the cpu.
   */
  void Start(int argc, char** argv);
  /**
   * Executes the loaded byte code.
   */
  void Run();

  inline void Push(double val);
  inline void Pop (uint8_t reg_id);

  inline void Out();

  inline void Add ();
  inline void Sub ();
  inline void Div ();
  inline void Mod ();
  inline void Sqrt();

  inline void End ();

  const char* GetDumpMessage(int error_code);


  char*   mem = new char[MEMORY_SIZE] {};
  int64_t reg   [REGISTERS_COUNT]     {};
  stack::SafeStackStatic<double, STACK_SIZE> stack;
};

}
}