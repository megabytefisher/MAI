
// runs through all MIPS commands inside a string
void run_from_string(mips_registers* registers, char* input)
{
    // infinite loop- until we run out of commands to process.
    while (1)
    {
        // first, skip all whitespace before the next command
        input = skip_whitespace(input);
        // read in the next keyword
        
    }
}

// used to skip whitespace characters.
// returns a modified character pointer from the input
char* skip_whitespace(char* input)
{
    while (*input == ' ' || *input == '\t' || *input == '\n')
    {
        input++;
    }
    return input;
}