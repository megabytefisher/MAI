#include "mips_memory.h"
#include "mips_instructions.h"
#include <string.h>
#include "utils.h"

#pragma once

void run_from_string(mips_state*, char*);
void run_from_data(char*);
extern char* LABEL_INFORMATION[20][2];
extern int arrayCounter;