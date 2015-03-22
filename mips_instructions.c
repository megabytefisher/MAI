#include "mips_instructions.h"

#include <stdio.h>

const char* INSTRUCTION_NAMES[] = { "help", "add", "addi", "and", "andi","subi","or", "ori" , "xor", "sllv", "slrv", "div", "mult", "noop", "mflo", "mfhi" };
const instruction_function INSTRUCTION_IMPLEMENTATION[] = { &help, &add, &addi, &and, &andi, &sub, &or, &ori, &xor, &sllv, &slrv, &div, &mult, &noop, &mflo, &mfhi};
const int INSTRUCTION_COUNT = 16;

typedef struct {
    int* destination_register;
    int* source_register_a;
    int* source_register_b;
    
    char* destination_register_string;
} r_instruction_data;

typedef struct {
    int* destination_register;
    
    char* destination_register_string;
} sr_instruction_data;

typedef struct {
    int* source_register_a;
    int* source_register_b;
    char* destination_register_string;
} dr_instruction_data;

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

void print_lo_register(mips_state* state)
{
    // the length of the register should always be 3 - because
    // $zero should never be modified
    
    printf("Modified register:\n\tlo");
    printf(" : %d\n", state->lo);
}

void print_hi_register(mips_state* state)
{
    // the length of the register should always be 3 - because
    // $zero should never be modified
    
    printf("Modified register:\n\thi");
    printf(" : %d\n", state->hi);
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

sr_instruction_data* parse_sr_instruction(sr_instruction_data* data, mips_state* state, char* parameters) {
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
    
    data->destination_register = destination_register;
    data->destination_register_string = destination_string;
    
    return data;
}

dr_instruction_data* parse_dr_instruction(dr_instruction_data* data, mips_state* state, char* parameters) {
    if (data == NULL)
        return NULL;
    
    // skip white space
    parameters = skip_over_whitespace(parameters);
    
    // get source register
    int* source_register_a = get_register_ptr(state, parameters);
    if (source_register_a == NULL)
    {
        printf("Error - first source is not a valid register!\n");
        return NULL;
    }
    
    // skip over the current identifier plus the whitespace after it
    parameters = skip_over_whitespace(skip_to_whitespace(parameters));
    
    // get source register
    int* source_register_b = get_register_ptr(state, parameters);
    if (source_register_b == NULL)
    {
        printf("Error - first source is not a valid register!\n");
        return NULL;
    }
    
    data->source_register_a = source_register_a;
    data->source_register_b = source_register_b;
    
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



void help(mips_state* state, char* parameters)
{
    printf("MIPS is a real-time MIPS interpreter.\nYou can enter MIPS assembly instruction and watch them execute one line at a time.\nFor example, try typing..\n\t>> addi $s0, $s0, 5\n\t>> add $s0, $s0, $s0\n");
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

void div(mips_state* state, char* parameters) {
    dr_instruction_data instruction_data;
    dr_instruction_data* parse_result = parse_dr_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    state->lo =
            *parse_result->source_register_a /
            *parse_result->source_register_b;
    
    state->hi = 
            *parse_result->source_register_a %
            *parse_result->source_register_b;
    
    // display modified register
    print_lo_register(state);
    print_hi_register(state);
}

void mult(mips_state* state, char* parameters){
    
     dr_instruction_data instruction_data;
    dr_instruction_data* parse_result = parse_dr_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    long result;
    result=
             *parse_result->source_register_a *
            *parse_result->source_register_b;
    
    state->lo =
            0xFFFFFFFF &
            result;
    
    state->hi=
            result>>
            32;
    
    
    // display modified register
    print_lo_register(state);
    print_hi_register(state);
    
}

void noop(mips_state* state, char* parameters) {
    // do nothing, best instruction
}

void mflo(mips_state* state, char* parameters) {
    sr_instruction_data instruction_data;
    sr_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    *parse_result->destination_register = 
            state->lo;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}

void mfhi(mips_state* state, char* parameters) {
    sr_instruction_data instruction_data;
    sr_instruction_data* parse_result = parse_r_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    *parse_result->destination_register = 
            state->hi;
    
    // display modified register
    print_modified_register(state, parse_result->destination_register_string);
}