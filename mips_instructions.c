#include "mips_instructions.h"

#include <stdio.h>

const char* INSTRUCTION_NAMES[] = { "add", "addi" };
const instruction_function INSTRUCTION_IMPLEMENTATION[] = { &add, &addi };
const int INSTRUCTION_COUNT = 2;

// utility function for printing whichever register was modified
void print_modified_register(mips_state* state, char* register_string)
{
    // the length of the register should always be 3 - because
    // $zero should never be modified
    
    int* register_value = get_register_ptr(state, register_string);
    
    printf("Modified register:\n\t");
    fwrite(register_string, 1, 3, stdout);
    printf(" : %d\n", *register_value);
}

// implementation for the standard add instruction
void add(mips_state* state, char* parameters)
{
    // skip white space
    parameters = skip_over_whitespace(parameters);
    
    // save this location in the string
    char* destination_string = parameters;
    
    // get the destination register
    int* destination_register = get_register_ptr(state, parameters);
    if (destination_register == NULL)
    {
        printf("Error - destination is not a valid register!\n");
        return;
    }
    if (destination_register == &state->zero)
    {
        printf("Error - you can't specify the $zero register as a destination!\n");
        return;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register A
    int* source_register_a = get_register_ptr(state, parameters);
    if (source_register_a == NULL)
    {
        printf("Error - first source is not a valid register!\n");
        return;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register B
    int* source_register_b = get_register_ptr(state, parameters);
    if (source_register_b == NULL)
    {
        printf("Error - second source is not a valid register!\n");
        return;
    }
    
    // add sources to destination
    *destination_register = *source_register_a + *source_register_b;
    
    // display modified register
    print_modified_register(state, destination_string);
}

void addi(mips_state* state, char* parameters)
{
    // skip white space
    parameters = skip_over_whitespace(parameters);
    
    // save this location in the string
    char* destination_string = parameters;
    
    // get the destination register
    int* destination_register = get_register_ptr(state, parameters);
    if (destination_register == NULL)
    {
        printf("Error - destination is not a valid register!\n");
        return;
    }
    if (destination_register == &state->zero)
    {
        printf("Error - you can't specify the $zero register as a destination!\n");
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register A
    int* source_register_a = get_register_ptr(state, parameters);
    if (source_register_a == NULL)
    {
        printf("Error - first source is not a valid register!\n");
        return;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // make sure this source isn't a register- it must be an immediate!
    if (*parameters == '$')
    {
        printf("Error - the third operand must be an immediate value, not a register!\n");
        return;
    }
    
    // TODO : maybe the value is a label?
    
    // read the value as an integer
    int immediate_value = atoi(parameters);
    
    // add sources to destination
    *destination_register = *source_register_a + immediate_value;
    
    // print modified destination
    print_modified_register(state, destination_string);
}