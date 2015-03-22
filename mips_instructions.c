#include "mips_instructions.h"

#include <stdio.h>

const char* INSTRUCTION_NAMES[] = { "add", "addi", "and", "andi","sub","subi","or", "ori" , "xor", "sllv", "slrv" };
const instruction_function INSTRUCTION_IMPLEMENTATION[] = { &add, &addi, &and, &andi, &sub, &subi, &or, &ori, &xor, &sllv, &slrv};
const int INSTRUCTION_COUNT = 11;

typedef struct {
    int* destination_register;
    int* source_register_a;
    int* source_register_b;
    
    char* destination_register_string;
} r_instruction_data;

typedef struct {
    int* destination_register;
    int* source_register;
    int immediate_value;
    
    char* destination_register_string;
} i_instruction_data;

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

r_instruction_data* parse_r_instruction(r_instruction_data* data, mips_state* state, char* parameters) {
    if (data == NULL)
        return NULL;
    
    // skip white space
    parameters = skip_over_whitespace(parameters);
    
    // save this location in the string
    char* destination_string = parameters;
    
    // get the destination register
    int* destination_register = get_register_ptr(state, parameters);
    if (destination_register == NULL)
    {
        printf("Error - destination is not a valid register!\n");
        return NULL;
    }
    if (destination_register == &state->zero)
    {
        printf("Error - you can't specify the $zero register as a destination!\n");
        return NULL;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register A
    int* source_register_a = get_register_ptr(state, parameters);
    if (source_register_a == NULL)
    {
        printf("Error - first source is not a valid register!\n");
        return NULL;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register B
    int* source_register_b = get_register_ptr(state, parameters);
    if (source_register_b == NULL)
    {
        printf("Error - second source is not a valid register!\n");
        return NULL;
    }
    
    data->destination_register = destination_register;
    data->source_register_a = source_register_a;
    data->source_register_b = source_register_b;
    data->destination_register_string = destination_string;
    
    return data;
}

i_instruction_data* parse_i_instruction(i_instruction_data* data, mips_state* state, char* parameters)
{
    if (data == NULL)
        return NULL;
    
    // skip white space
    parameters = skip_over_whitespace(parameters);
    
    // save this location in the string
    char* destination_string = parameters;
    
    // get the destination register
    int* destination_register = get_register_ptr(state, parameters);
    if (destination_register == NULL)
    {
        printf("Error - destination is not a valid register!\n");
        return NULL;
    }
    if (destination_register == &state->zero)
    {
        printf("Error - you can't specify the $zero register as a destination!\n");
        return NULL;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register A
    int* source_register_a = get_register_ptr(state, parameters);
    if (source_register_a == NULL)
    {
        printf("Error - first source is not a valid register!\n");
        return NULL;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // make sure this source isn't a register- it must be an immediate!
    if (*parameters == '$')
    {
        printf("Error - the third operand must be an immediate value, not a register!\n");
        return NULL;
    }
    
    // TODO : maybe the value is a label?
    
    // read the value as an integer
    int immediate_value = atoi(parameters);
    
    data->destination_register = destination_register;
    data->source_register = source_register_a;
    data->destination_register_string = destination_string;
    data->immediate_value = immediate_value;
    
    return data;
}



// implementation for the standard add instruction
void add(mips_state* state, char* parameters)
{
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // add sources to destination
    *parse_result->destination_register =
            *parse_result->source_register_a +
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}

void addi(mips_state* state, char* parameters)
{
    i_instruction_data instruction_data;
    i_instruction_data* parse_result = parse_i_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // add sources to destination
    *parse_result->destination_register =
            *parse_result->source_register +
            parse_result->immediate_value;
    
    // print modified destination
    print_modified_register(state, parse_result->destination_register_string);
}

void and(mips_state* state, char* parameters) {
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }

    
    // AND sources to destination
    *parse_result->destination_register =
            *parse_result->source_register_a &
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}

void andi(mips_state* state, char* parameters) {
    i_instruction_data instruction_data;
    i_instruction_data* parse_result = parse_i_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // AND immediate sources to destination
    *parse_result->destination_register =
            *parse_result->source_register &
            parse_result->immediate_value;
    
    // print modified destination
    print_modified_register(state, parse_result->destination_register_string);
}

void sub(mips_state* state, char* parameters)
{
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // sub sources to destination
    *parse_result->destination_register =
            *parse_result->source_register_a -
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}


void subi(mips_state* state, char* parameters)
{
    i_instruction_data instruction_data;
    i_instruction_data* parse_result = parse_i_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // sub immediate sources to destination
    *parse_result->destination_register =
            *parse_result->source_register -
            parse_result->immediate_value;
    
    // print modified destination
    print_modified_register(state, parse_result->destination_register_string);
}


void or(mips_state* state, char* parameters) {
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }

    
    // AND sources to destination
    *parse_result->destination_register =
            *parse_result->source_register_a |
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}




void ori(mips_state* state, char* parameters) {
    i_instruction_data instruction_data;
    i_instruction_data* parse_result = parse_i_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // or immediate sources to destination
    *parse_result->destination_register =
            *parse_result->source_register |
            parse_result->immediate_value;
    
    // print modified destination
    print_modified_register(state, parse_result->destination_register_string);
}


void xor(mips_state* state, char* parameters) {
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }

    
    // XOR sources to destination
    *parse_result->destination_register =
            *parse_result->source_register_a ^
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}

void sllv(mips_state* state, char* parameters) {
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    *parse_result->destination_register =
            *parse_result->source_register_a <<
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}

void slrv(mips_state* state, char* parameters) {
    r_instruction_data instruction_data;
    r_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    *parse_result->destination_register =
            *parse_result->source_register_a >>
            *parse_result->source_register_b;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}