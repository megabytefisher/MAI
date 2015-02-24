// mips_memory.c
// Contains functions for managing/modifying the MIPS registers and stack

#include "mips_memory.h"
#include <stdlib.h>
#include <string.h>

void initialize_stack(mips_state *registers) {
    // allocate 1024 kilobytes (1 MB) for stack space
    registers->sp = (int)malloc(1024 * 1024);
    // save the bottom of the stack so we can destroy it later
    registers->stackBottom = (void*)registers->sp;
}

void destroy_stack(mips_state *registers) {
    // free the memory that is held by the stack
    free(registers->stackBottom);
    // set the addresses to zero
    registers->stackBottom = (void*)0;
    registers->sp = 0;
}

// gets a pointer to a register variable by its name in string form
int* get_register_ptr(mips_state *state, char* registerName) {
    // the first letter of a register name must start with $
    if (*registerName != '$')
        return NULL;
    // skip over that $
    registerName++;
    
    // now we check if the opcode is each of the possible strings...
    // and if it matches, return a pointer to the data
    if (memcmp(registerName, "zero", 4) == 0)
        return &(state->zero);
    else if (memcmp(registerName, "at", 2))
        return &(state->at);
    else if (memcmp(registerName, "v0", 2))
        return &(state->v0);
    else if (memcmp(registerName, "v1", 2))
        return &(state->v1);
    else if (memcmp(registerName, "a0", 2))
        return &(state->a0);
    else if (memcmp(registerName, "a1", 2))
        return &(state->a1);
    else if (memcmp(registerName, "a2", 2))
        return &(state->a2);
    else if (memcmp(registerName, "a3", 2))
        return &(state->a3);
    else if (memcmp(registerName, "t0", 2))
        return &(state->t0);
    else if (memcmp(registerName, "t1", 2))
        return &(state->t1);
    else if (memcmp(registerName, "t2", 2))
        return &(state->t2);
    else if (memcmp(registerName, "t3", 2))
        return &(state->t3);
    else if (memcmp(registerName, "t4", 2))
        return &(state->t4);
    else if (memcmp(registerName, "t5", 2))
        return &(state->t5);
    else if (memcmp(registerName, "t6", 2))
        return &(state->t6);
    else if (memcmp(registerName, "t7", 2))
        return &(state->t7);
    else if (memcmp(registerName, "s0", 2))
        return &(state->s0);
    else if (memcmp(registerName, "s1", 2))
        return &(state->s1);
    else if (memcmp(registerName, "s2", 2))
        return &(state->s2);
    else if (memcmp(registerName, "s3", 2))
        return &(state->s3);
    else if (memcmp(registerName, "s4", 2))
        return &(state->s4);
    else if (memcmp(registerName, "s5", 2))
        return &(state->s5);
    else if (memcmp(registerName, "s6", 2))
        return &(state->s6);
    else if (memcmp(registerName, "s7", 2))
        return &(state->s7);
    else if (memcmp(registerName, "s8", 2))
        return &(state->s8);
    else if (memcmp(registerName, "s9", 2))
        return &(state->s9);
    else if (memcmp(registerName, "k0", 2))
        return &(state->k0);
    else if (memcmp(registerName, "k1", 2))
        return &(state->k1);
    else if (memcmp(registerName, "gp", 2))
        return &(state->gp);
    else if (memcmp(registerName, "sp", 2))
        return &(state->sp);
    else if (memcmp(registerName, "fp", 2))
        return &(state->fp);
    else if (memcmp(registerName, "ra", 2))
        return &(state->ra);
    
    return NULL;
}