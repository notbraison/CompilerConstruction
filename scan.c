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

    for(int i = 0; i < 6; i++)
        keyword_max_length[i] = strlen(keywords);

    struct found_token next_token;
    next_token.token_start = input;

    if (*input == '\0')
    {
        printf("End of input string reached\n");
        next_token.token_type = ERROR;
        return next_token;
    }

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
            {
                if(check_char_in_range(*(input + index), identifier_legal_characters, identifiers_length))
                    next_token.token_type = IDENTIFIER;
                else
                    next_token.token_type = ERROR;

                index++;
            }

            break;
        case CLASS_KEYWORD:
            while(!check_char_in_range(*(input + index), delimiters, delimiters_size))
            {
                if(check_char_in_range(*(input + index), keywords[index], keyword_max_length[index]))
                    next_token.token_type = KEYWORD;
                else if(check_char_in_range(*(input + index), identifier_legal_characters, identifiers_length))
                    next_token.token_type = IDENTIFIER;
                else
                    next_token.token_type = ERROR;

                index++;
            }

            break;
        case CLASS_QUOTEMARK:
            while(*(input + index) != '\n')
            {
                if(*(input + index) == '\"')
                    break;

                index++;
            }

            next_token.token_type = STRING;

            break;
        case CLASS_OPERATOR:
            if(!check_char_in_range(*(input + index), delimiters, delimiters_size))
                if(check_char_in_range(*(input + index), operators[index], operator_length[index]))
                    index++;

            next_token.token_type = OPERATOR;
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

    int first_list = 0;
    keyword_max_length[first_list] = strlen(keywords[first_list]);
    operator_length[first_list] = strlen(operators[first_list]);

    if (check_char_in_range(character, integer_characters, integers_length))
        return CLASS_INTEGER;

    if(check_char_in_range(character, keywords[0], keyword_max_length[0]))
        return CLASS_KEYWORD;

    if (check_char_in_range(character, identifier_legal_characters, identifiers_length))
        return CLASS_ALPHABET;

    if(check_char_in_range(character, operators[first_list], operator_length[first_list]))
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
