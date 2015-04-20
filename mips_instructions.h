#include "mips_memory.h"
#include "utils.h"

#pragma once

// function pointer to a mips instruction implementation
typedef void (*instruction_function)(mips_state*, char*);
typedef void (*data_function)(char*);

// instruction implementations
void help(mips_state*, char*);
void add(mips_state*, char*);
void addi(mips_state*, char*);
void and(mips_state*, char*);
void andi(mips_state*, char*);
void sub(mips_state*, char*);
void or(mips_state*, char*);
void ori(mips_state*, char*);
void xor(mips_state*, char*);
void sllv(mips_state*, char*);
void slrv(mips_state*, char*);
void noop(mips_state*, char*);
void divi(mips_state*, char*); // div command, but cannot be named div due to conflict
void mult(mips_state*, char*);
void mflo(mips_state*, char*);
void mfhi(mips_state*, char*);
void li(mips_state*, char*);
void syscall(mips_state*, char*);
void jr(mips_state* state, char* parameters);
void j(mips_state* state, char* parameters);
void beq(mips_state* state, char* parameters);
void bgez(mips_state* state, char* parameters);
void bgezal(mips_state* state, char* parameters);
void bgtz(mips_state* state, char* parameters);
void blez(mips_state* state, char* parameters);
void bltz(mips_state* state, char* parameters);
void bltzal(mips_state* state, char* parameters);
void bne(mips_state* state, char* parameters);
void lui(mips_state* state, char* parameters);

// pseudo-dictionary for going from char* -> function pointer
extern const char* INSTRUCTION_NAMES[];
extern const instruction_function INSTRUCTION_IMPLEMENTATION[];
extern const int INSTRUCTION_COUNT,NT;

