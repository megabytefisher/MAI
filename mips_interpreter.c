#include "mips_interpreter.h"
#include <string.h>

// runs through all MIPS commands inside a string

void run_from_string(mips_state* registers, char* input) {
    // infinite loop- until we run out of commands to process.
    while (input != NULL) {
        // first, skip all whitespace before the next command
        input = skip_over_whitespace(input);

        // now we need to parse the non-whitespace text.
        // if could be and opcode, label, or section specifier

        // first we check if its an opcode.
        // we do this by scanning through the array of stored instruction names, and memcmping the current text
        for (int i = 0; i < INSTRUCTION_COUNT; i++) {
            int instruction_name_length = strlen(INSTRUCTION_NAMES[i]);
            if (memcmp(INSTRUCTION_NAMES[i], input, instruction_name_length) == 0) {
                // make sure there is at least one character of whitespace after the command.
                // if we don't check this, we might get false positives- add will trigger addi, etc..
                if (!is_whitespace(input + instruction_name_length))
                    continue;

                // we got a match!

                // look up the function in the implementation array
                instruction_function opcode_func = INSTRUCTION_IMPLEMENTATION[i];
                // skip over the instruction name
                input += instruction_name_length;
                // call that bad boy
                opcode_func(registers, input);
            }
        }

        // we didn't find any matching opcode name. maybe it's a section specifier?
        break;
    }
}

void run_from_data(char* input) {

    // infinite loop- until we run out of commands to process.
    while (input != NULL) {

        //char delimiter1[2] = "\"";
        char delimiter2[2] = ":";
        
        char bufferData[256];

        int arrayCounter = 0;
        char *token1 = strstr(input, "\"");
        strcpy(bufferData, token1);
        char *subtoken2a = strtok(input, "\"");
        char *token2 = strstr(subtoken2a, ".");
        char *token3 = strtok(input, delimiter2);
        
        int test = strcmp(token2, ".data");
        printf("%d\n", test);
        
        if (strcmp(token2, ".asciiz") == 32) {

            token1++;//get rid of the first quotation
            int token1Length=strlen(token1);
            token1[token1Length-2]=0;
            
            //STORE STRING INSIDE QUOTES
            while (token1 != NULL) {
                
                LABEL_INFORMATION[arrayCounter][0]=strdup(token1);
                LABEL_INFORMATION[arrayCounter][1]=strdup(token3);
                arrayCounter++;

                //LABEL_INFORMATION[0][0] = strdup(token1);
                break;
            }

        //} else if (  ) {
            
        }else {
            //STORE INTEGER STRAIGHT TO ARRAY
           
                //LABEL_INFORMATION[0][0] = strdup(token1);
                break;
            }
        



        //printf("%d", LABEL_INFORMATION[0][0]);
        // first, skip all whitespace before the next command
        // input = skip_over_whitespace(input);

        // now we need to parse the non-whitespace text.
        // if could be and opcode, label, or section specifier

        // first we check if its an opcode.
        // we do this by scanning through the array of stored instruction names, and memcmping the current text


        // we didn't find any matching opcode name. maybe it's a section specifier?
        break;
    }
}