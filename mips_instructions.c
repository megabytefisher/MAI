#include "mips_instructions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* INSTRUCTION_NAMES[] = {"syscall", "help", "add", "addi", "and", "andi","sub","or", "ori" , "xor", "sllv", "slrv", "div", "mult", "noop", "mflo", "mfhi", "li", "jr", "j", "beq", "bgez", "bgezal", "bgtz", "blez", "bltz", "bltzal", "bne" };
const instruction_function INSTRUCTION_IMPLEMENTATION[] = {&syscall, &help, &add, &addi, &and, &andi, &sub, &or, &ori, &xor, &sllv, &slrv, &divi, &mult, &noop, &mflo, &mfhi, &li, &jr, &j, &beq, &bgez, &bgezal, &bgtz, &blez, &bltz, &bltzal, &bne };
const int INSTRUCTION_COUNT = 28;

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
    int immediate_value;
    
    char* destination_register_string;
} dri_instruction_data;

typedef struct {
    int immediate_value;
} si_instruction_data;


typedef struct {
    int* destination_register;
    int* source_register;
    int immediate_value;
    
    char* destination_register_string;
} i_instruction_data;

int read_immediate(mips_state* state, char* input)
{
    // an immediate can either be an integer value or specify a label to read from data section
    
    // first let's see if its a label
    data_entry* node = state->data_section;
    while (node != 0)
    {
        // check if the input matches
        int name_length = strlen(node->name);
        if(memcmp(input, node->name, name_length) == 0)
        {
            break;
        }
        
        node = node->next;
    }
    
    if (node != 0)
    {
        // we found a matching node!
        
        // if it is an integer value, we just return the value of the string
        if (strcmp(".word", node->type) == 0 || strcmp(".halfword", node->type) == 0 || strcmp(".byte", node->type) == 0)
        {
            int value = atoi(node->data);
            return value;
        }
        // else it must be either a string or space, so we return the address
        return (int)node->data;
    }
    
    // maybe it is a label specifying an instruction address??
    text_entry* text_node = state->text_section;
    int text_address = 0;
    while (text_node != 0)
    {
        int name_length = strlen(text_node->label);
        if (name_length > 0)
        {
            if (memcmp(input, text_node->label, name_length) == 0)
            {
                // it's a match, return the address of the instruction!
                return text_address;
            }
        }
        
        // keep looking
        text_node = text_node->next;
        // increment address by 4
        text_address += 4;
    }
    
    // so it did not match any node.. just parse it as an integer value
    return atoi(input);
}

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
    
    // read the immediate
    int immediate_value = read_immediate(state, parameters);
    
    data->destination_register = destination_register;
    data->source_register = source_register_a;
    data->destination_register_string = destination_string;
    data->immediate_value = immediate_value;
    
    return data;
}


dri_instruction_data* parse_dri_instruction(dri_instruction_data* data, mips_state* state, char* parameters)
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
    
    
    // make sure this source isn't a register- it must be an immediate!
    if (*parameters == '$')
    {
        printf("Error - the third operand must be an immediate value, not a register!\n");
        return NULL;
    }
    
    // read immediate
    int immediate_value = read_immediate(state, parameters);
    
    data->destination_register = destination_register;
    data->destination_register_string = destination_string;
    data->immediate_value = immediate_value;
    
    return data;
}

si_instruction_data* parse_si_instruction(si_instruction_data* data, mips_state* state, char* parameters)
{
    if (data == NULL)
        return NULL;
    
    // skip white space
    parameters = skip_over_whitespace(parameters);
    
    // make sure this source isn't a register- it must be an immediate!
    if (*parameters == '$')
    {
        printf("Error - this instruction expects an immediate value, not a register!\n");
        return NULL;
    }
    
    // read immediate
    int immediate_value = read_immediate(state, parameters);
    
    data->immediate_value = immediate_value;
    
    return data;
}

void help(mips_state* state, char* parameters)
{
    printf("MIPS is a real-time MIPS interpreter.\nYou can enter MIPS assembly instruction and watch them execute one line at a time.\nFor example, try typing..\n\t>> addi $s0, $s0, 5\n\t>> add $s0, $s0, $s0\n");
}

void syscall(mips_state* state, char* parameters)
{
    // the length of the register should always be 3 - because
    // $zero should never be modified
      int printa0= state->a0;
    switch(state->v0){
        
        case 1:
            printf("Printing register a0\n");
            printf("%d\n", printa0);
                break;
        case 2:
                break; 
        case 3:
                break; 
        case 4:
                break;
            
        case 5:
            
            printf("Please enter value to store in $v0:");
            int n;
            scanf("%d", &n);
            state->v0=n;
             printf("Modified register:\n\t$v0");
             printf(" : %d\n", state->v0);
            break;
            
        case 10:
            printf("MAI program ending. Goodbye.");
            exit(0);
            break;
            
        case 12:
           
            printf("Please enter a character to store in $v0:");
            state->v0= getchar();
            printf("Modified register:\n\t$v0:");
            putchar(state->v0);
            printf("\n");
            break;
    
    }
 
   
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

void divi(mips_state* state, char* parameters) {
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
    sr_instruction_data* parse_result = parse_sr_instruction(&instruction_data, state, parameters);
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

void li(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    if (parse_result == NULL)
    {
        return;
    }
    
    // add sources to destination
    *parse_result->destination_register =
            parse_result->immediate_value;
    
    // print modified destination
    print_modified_register(state, parse_result->destination_register_string);
}

void jr(mips_state* state, char* parameters)
{
    sr_instruction_data instruction_data;
    sr_instruction_data* parse_result = parse_sr_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
    {
        return;
    }
    
    // set pc to register specified
    state->pc = *parse_result->destination_register;
    
    printf("Modified register:\n\tpc : %d\n", state->pc);
}

void j(mips_state* state, char* parameters)
{
    si_instruction_data instruction_data;
    si_instruction_data* parse_result = parse_si_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
    {
        return;
    }
    
    // jump to immediate value
    state->pc = parse_result->immediate_value;
    
    printf("Modified register:\n\tpc : %d\n", state->pc);
}

void jal(mips_state* state, char* parameters)
{
    si_instruction_data instruction_data;
    si_instruction_data* parse_result = parse_si_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
    {
        return;
    }
    
    // save pc (already +4) to ra
    state->ra = state->pc;
    // jump to immediate value
    state->pc = parse_result->immediate_value;
    
    printf("Modified register:\n\tpc : %d\n", state->pc);
}

void beq(mips_state* state, char* parameters)
{
    i_instruction_data instruction_data;
    i_instruction_data* parse_result = parse_i_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->source_register == *parse_result->destination_register)
    {
        // they are equal, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void bgez(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register >= 0)
    {
        // greater or equal to 0, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void bgezal(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register >= 0)
    {
        // save return address
        state->ra = state->pc;
        // greater or equal to 0, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void bgtz(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register > 0)
    {
        // greater than 0, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void blez(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register <= 0)
    {
        // less than or equal to 0, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void bltz(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register < 0)
    {
        // less than zero, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void bltzal(mips_state* state, char* parameters)
{
    dri_instruction_data instruction_data;
    dri_instruction_data* parse_result = parse_dri_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register < 0)
    {
        // save return address
        state->ra = state->pc;
        // less than 0, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}

void bne(mips_state* state, char* parameters)
{
    i_instruction_data instruction_data;
    i_instruction_data* parse_result = parse_i_instruction(&instruction_data, state, parameters);
    
    if (parse_result == NULL)
        return;
    
    if (*parse_result->destination_register != *parse_result->source_register)
    {
        // they are not equal, now jump
        state->pc = parse_result->immediate_value;
        
        printf("Modified register:\n\tpc : %d\n", state->pc);
    }
}