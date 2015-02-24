#include "mips_memory.h"
#include "utils.h"

#pragma once

// function pointer to a mips instruction implementation
typedef void (*instruction_function)(mips_state*, char*);

// instruction implementations
void add(mips_state*, char*);

// pseudo-dictionary for going from char* -> function pointer
extern const char* INSTRUCTION_NAMES[];
extern const instruction_function INSTRUCTION_IMPLEMENTATION[];
extern const int INSTRUCTION_COUNT;