#include "mips_interpreter.h"
#include <stdio.h>

// saves an instruction to the text section memory
void save_to_text(mips_state* state, char* input)
{
    // increment current instruction "address" by 4
    state->instruction_counter += 4;
    
    // check and see if text section exists yet
    if (state->text_section == NULL)
    {
        // create a new node
        text_entry* node = malloc(sizeof(text_entry));
        strcpy(node->instruction, input);
        node->next = 0;
        
        state->text_section = node;
    }
    else
    {
        // we need to find where to place the new node
        text_entry* node = state->text_section;
        // scan to end of linked-list
        while (node->next != 0)
            node = node->next;
        
        node->next = malloc(sizeof(text_entry));
        strcpy(node->next->instruction, input);
        node->next->next = 0;
    }
}

// returns a pointer to a function to execute given an opcode name
instruction_function get_instruction_function(char* input, int* name_length)
{
    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        int instruction_name_length = strlen(INSTRUCTION_NAMES[i]);
        if (memcmp(INSTRUCTION_NAMES[i], input, instruction_name_length) == 0)
        {
            if (!is_whitespace(input + instruction_name_length))
                continue;
            
            instruction_function opcode_func = INSTRUCTION_IMPLEMENTATION[i];
            *name_length = instruction_name_length;
            return opcode_func;
        }
    }
    
    return 0;
}

void read_into_data(mips_state* state)
{
    printf("Entering data section input mode (enter .text to exit):\n");
    while(1)
    {
        printf("\t");
        char inputLine[56];
        char* p;

        // read next input line
        p = fgets(inputLine, sizeof(inputLine), stdin);
        
        if (memcmp(inputLine, ".text", 5) == 0)
        {
            // exit this mode
            break;
        }
        
        data_entry* node = malloc(sizeof(data_entry));
        
        // name is everything before :
        char* name = strtok(p, ":");
        strcpy(node->name, name);
        // type is everything one space after that + everything to the next space
        p += strlen(name) + 1;
        char* type = p;
        type = strtok(type, " ");
        strcpy(node->type, type);
        // value can be tricky- if it is a string it must start and end with ", otherwise it is just the data.
        p += strlen(type) + 2;
        // p holds value, but if its a string, we should remove the first and last "
        if (*p == '"' && p[strlen(p) - 2] == '"')
        {
            p++;
            p[strlen(p) - 2] = '\0';
        }
        
        
        printf("inputLine = '%s'\n", p);
    }
}

// runs through all MIPS commands inside a string
void run_from_string(mips_state* registers, char* input)
{
    // infinite loop- until we run out of commands to process.
    if (input != NULL)
    {
        // first, skip all whitespace before the next command
        input = skip_over_whitespace(input);
        
        // now we need to parse the non-whitespace text.
        // if could be and opcode, label, or section specifier
        
        int instruction_name_length;
        instruction_function c_func = get_instruction_function(input, &instruction_name_length);
        
        if (c_func == 0)
        {
            // the input isn't a valid opcode
            
            // maybe it is specifying a .data mode change?
            if (memcmp(input, ".data", 5) == 0)
            {
                read_into_data(registers);
            }
            return;
        }
                
        // increment pc by 4
        registers->pc += 4;

        // skip over the instruction name
        char* input_args = input + instruction_name_length;
        // call that bad boy
        c_func(registers, input_args);

        // save instruction text
        save_to_text(registers, input);
    }
}

void run_from_pc(mips_state* state)
{
    // first we must find the starting location
    text_entry* node = state->text_section;
    for (int i = 0; i < state->pc; i+=4)
    {
        node = node->next;
    }
    
    // got the starting node.. now just run from it.
    while (state->pc != state->instruction_counter)
    {
        // increment pc by 4
        state->pc += 4;
        
        int instruction_name_length;
        instruction_function c_func = get_instruction_function(node->instruction, &instruction_name_length);
        
        c_func(state, (node->instruction) + instruction_name_length);
        
        // move to next node*/
        if (node->next != 0)
            node = node->next;
    }
}