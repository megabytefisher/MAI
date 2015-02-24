#include "mips_interpreter.h"

// runs through all MIPS commands inside a string
void run_from_string(mips_state* registers, char* input)
{
    // infinite loop- until we run out of commands to process.
    while (1)
    {
        // first, skip all whitespace before the next command
        input = skip_over_whitespace(input);
        
        // now we need to parse the non-whitespace text.
        // if could be and opcode, label, or section specifier
        
        // first we check if its an opcode.
        // we do this by scanning through the array of stored instruction names, and memcmping the current text
        for (int i = 0; i < INSTRUCTION_COUNT; i++)
        {
            int instruction_name_length = strlen(INSTRUCTION_NAMES[0]);
            if (memcmp(INSTRUCTION_NAMES[0], input, instruction_name_length) == 0)
            {
                // we got a match!
                
                // look up the function in the implementation array
                instruction_function opcode_func = INSTRUCTION_IMPLEMENTATION[i];
                // call that bad boy
                opcode_func(registers, input + instruction_name_length);
            }
        }
        
        // we didn't find any matching opcode name. maybe it's a section specifier?
        
        break;
    }
}