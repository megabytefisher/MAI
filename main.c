// main.c for MAI (MIPS Assembly Interpreter)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mips_memory.h"
#include "mips_interpreter.h"

int main(int argc, char** argv) {
    // create the MIPS register space, and set all internal variables to zero
    mips_state registers = { 0 };
    // create space for the MIPS stack
    initialize_stack(&registers);
    
    if (argc == 1) // no parameters. run in live interpreter mode.
    {
        printf("Welcome to MAI (MIPS Assembly Interpreter).\n");
        printf("Enter commands to execute and press enter to execute. For help, execute 'help'.\n");

        // read next input line
        while (1)
        {
            // if pc equals inst counter, then we need new input from user
            if (registers.pc == registers.instruction_counter)
            {
                printf("%d>> ", registers.instruction_counter);
                char inputLine[56];
                char* p;

                // read next input line
                p = fgets(inputLine, sizeof(inputLine), stdin);
                run_from_string(&registers, inputLine);
            }
            else if (registers.pc > registers.instruction_counter)
            {
                printf("Error - pc ended up higher than possible. Resetting it to highest possible value.\n");
                printf("pc was %d\n", registers.pc);
                registers.pc = registers.instruction_counter;
            }
            else
            {
                run_from_pc(&registers);
            }
        }
    }
    
    return (EXIT_SUCCESS);
}

