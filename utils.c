

// checks if the specified character is whitespace
int is_whitespace(char* input)
{
    if (*input == ' ' || *input == '\t' || *input == '\n' || *input == ',')
    {
        return 1;
    }
    return 0;
}

// used to skip whitespace characters.
// returns a modified character pointer from the input
char* skip_over_whitespace(char* input)
{
    while (is_whitespace(input))
    {
        input++;
    }
    return input;
}

// used to skip over non-whitespace characters
// basically the opposite of skip_over_whitespace
char* skip_to_whitespace(char* input)
{
    while (!is_whitespace(input))
    {
        input++;
    }
    return input;
}