#include "mips_memory.h"
#include "mips_instructions.h"
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#pragma once

void run_from_string(mips_state*, char*);
void run_from_pc(mips_state* state);