#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// a header file containing the global definitions
#include "global_declares.h"

struct found_token tokenScanner(char *input)
{
    bool check_char_in_range(char character, char *range, int length);
    enum characterclass intial_token_class(char character);

    static int current_character = 0;
    input += current_character;
    int index = 0;
    enum characterclass initial_class;
    bool period_found = false;

    struct found_token next_token;
    next_token.token_start = input;

    if (*input == '\0')
    {
        printf("End of input string reached\n");
        next_token.token_type = ERROR;
        return next_token;
    }

    if (index == 0)
        initial_class = initial_token_class(*input);
    index++;

    switch (initial_class)
    {
        case CLASS_INTEGER:
            while (!check_char_in_range(*(input + index), delimiters, delimiters_size))
            {
                if(*(input + index) == period)
                {
                    if(period_found)
                    {
                        next_token.token_type = ERROR;
                        break;
                    }
                    next_token.token_type = DOUBLE;
                } 
                
                if(check_char_in_range(*(input + index), integer_characters, integers_length))
                {
                    if(!period_found)
                        next_token.token_type = INTEGER;
                }
                else
                {
                    next_token.token_type = ERROR;
                    break;
                }

                index++;
            }
            
            break;
        case CLASS_ALPHABET:
            while (!check_char_in_range(*(input + index), delimiters, delimiters_size))
            {}
            break;
        case CLASS_QUOTEMARK:
            while(*(input + index) != '\"')
                index++;
            next_token.token_type = STRING;
        case CLASS_OPERATOR:
            break;
        default:
            next_token.token_type = ERROR;
            break;
    }

    next_token.token_length = index;
    current_character += index;
}

enum characterclass intial_token_class(char character)
{
    bool check_char_in_range(char character, char *range, int length);

    if (check_char_in_range(character, integer_characters, integers_length))
        return CLASS_INTEGER;

    if(check_char_in_range(character, keywords[0], NUMBER_OF_KEYWORDS))
        return CLASS_KEYWORD;

    if (check_char_in_range(character, identifier_legal_characters, identifiers_length))
        return CLASS_ALPHABET;

    for(int i = 0; i < NUMBER_OF_OPERATORS; i++)
        if(character == operators[i])
            return CLASS_OPERATOR;

    if (character == "\"")
        return CLASS_QUOTEMARK;

    return CLASS_ERROR;
}

bool check_char_in_range(char character, char *range, int length)
{
    // check if the current character is a delimiter
    for (int i = 0; i < length; i++)
        if (character == range[i])
            // if it is return true
            return true;

    // if not return false
    return false;
}

// Function to check if a string is a keyword
int check_keyword(char *word)
{
    char keywords[7][10] = {"if", "else", "while", "int", "double", "bool", "String"};
    for (int i = 0; i < 7; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        { // used for comparing two strings
            return 1;
        }
    }
    return 0;
}

// Function to classify token type
TokenType classify_tokens(char *lexeme)
{
    int check_keyword(char *word);

    char first_character = lexeme[0];

    // check if first character is an alphabetical character or an underscore first
    if (isalpha(first_character) || first_character == '_')
    {
        return check_keyword(lexeme) ? KEYWORD : IDENTIFIER;
    }
    // check if first character is a digit
    else if (isdigit(first_character))
    {
        // Check if lexeme is a double constant
        if (strchr(lexeme, '.') != NULL)
        {
            // strchr function in C is used to search for the first occurrence of a specified character in a given string
            return DOUBLE;
        }
        else
        {
            return INTEGER;
        }
    }
    // check if first character is quotation mark
    else if (first_character == '"')
    {
        // Check if lexeme is a string
        return STRING;
    }
    else
    {
        // Check for operators and boolean constants
        char operators[18][3] = {"+", "-", "*", "/", "%%", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="};
        for (int i = 0; i < 18; i++)
        {
            if (strcmp(lexeme, operators[i]) == 0)
            {
                return OPERATOR;
            }
        }
        // Check for boolean constants
        if (strcmp(lexeme, "true") == 0 || strcmp(lexeme, "false") == 0)
        {
            return BOOLEAN;
        }
        return ERROR;
    }
}

// Function to scan tokens from input
void scan_tokens(char *input)
{
    TokenType classify_tokens(char *lexeme);

    // these delimiters are used to signal the end of a token
    char delimiters[] = " \t\n{};()";
    // strtok function in C is used to tokenize (i.e., break down) a string into smaller strings
    char *token = strtok(input, delimiters);

    while (token != NULL)
    {
        TokenType type = classify_tokens(token);
        printf("Token: %s\tType: ", token);
        switch (type)
        {
        case IDENTIFIER:
            printf("Identifier\n");
            break;
        case KEYWORD:
            printf("Keyword\n");
            break;
        case INTEGER:
            printf("Integer\n");
            break;
        case DOUBLE:
            printf("Double\n");
            break;
        case STRING:
            printf("String\n");
            break;
        case OPERATOR:
            printf("Operator\n");
            break;
        case BOOLEAN:
            printf("Boolean\n");
            break;
        default:
            printf("Error\n");
            break;
        }
        token = strtok(NULL, delimiters);
    }
}

int get_source_code(char *filename, char *source_text, FILE *source_file_ptr)
{
    char input_symbol; // temporary storage for characters from the file
    int index = 0;     // loop counter
    int result = 0;

    // Check if the file could be opened
    if ((source_file_ptr = fopen(filename, "r")) == NULL)
    {
        printf("Could not open file, %s", filename);
        return result;
    }
    result = 1;

    // getting text from the file and putting in given string
    while ((input_symbol = fgetc(source_file_ptr)) != EOF)
    {
        source_text[index] = input_symbol;
        printf("%c", input_symbol);
        index++;
    }
}

int main(int argc, char **argv)
{
    FILE *source_file_ptr; // file pointer to the source code file
    char *source_file_name;
    int CMD_args = 2;
    int filename_index = 1;
    char source_text[SOURCE_CODE_LIMIT];

    int get_source_code(char *filename, char *source_text, FILE *source_file_ptr);

    if (argc != CMD_args)
    {
        printf("Wrong number of arguements");
        exit(EXIT_FAILURE);
    }

    source_file_name = argv[filename_index];

    // obtain the source code from a file
    if (!get_source_code(source_file_name, source_text, source_file_ptr))
        exit(EXIT_FAILURE);

    printf("Scanning input program:\n%s\n", source_text);
    printf("==============================================\n");

    // run the scanner and get back the results
    scan_tokens(source_text);
    fclose(source_file_ptr);
    exit(EXIT_SUCCESS);
}
