
// used to skip whitespace characters.
// returns a modified character pointer from the input
char* skip_over_whitespace(char* input)
{
    while (*input == ' ' || *input == '\t' || *input == '\n' || *input == ',')
    {
        input++;
    }
    return input;
}

// used to skip over non-whitespace characters
// basically the opposite of skip_over_whitespace
char* skip_to_whitespace(char* input)
{
    while (*input != ' ' && *input != '\t' && *input != '\n' || *input == ',')
    {
        input++;
    }
    return input;
}
