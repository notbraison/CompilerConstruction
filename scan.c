#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// header file for global declarations
#include "global_declares.h"

int main(int argc, char **argv)
{
    FILE *source_file_ptr; // file pointer to the source code file
    char *source_file_name;
    int CMD_args = 2;
    int filename_index = 1;
    char source_text[SOURCE_CODE_LIMIT];

    int getSourceCode(char *filename, char *source_text, FILE *source_file_ptr);
    struct found_token tokenScanner(char *input);
    void tokenStream(char *input);

    if (argc != CMD_args)
    {
        printf("Wrong number of arguements");
        exit(EXIT_FAILURE);
    }

    source_file_name = argv[filename_index];

    // obtain the source code from a file
    if (!getSourceCode(source_file_name, source_text, source_file_ptr))
        exit(EXIT_FAILURE);

    printf("Scanning input program:\n%s\n", source_text);
    printf("==============================================\n");
    printf("Token\t  Type\n");

    // run the scanner and get back the results
    tokenStream(source_text);

    fclose(source_file_ptr);
    exit(EXIT_SUCCESS);
}

void tokenStream(char *input)
{
    struct found_token tokenScanner(char *input);
    bool stop = false;
    int index = 0;

    do
    {
        struct found_token current_token = tokenScanner(input);

        switch (current_token.token_type)
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
        case ERROR:
            stop = true;
            printf("\n");
        default:
            printf("Unkown\n");
            break;
        }

        tokens_found[index] = current_token;
        index++;
    } while (!stop);
}

struct found_token tokenScanner(char *source_text)
{
    bool checkCharInRange(char character, char *range, int length);
    enum characterclass initialTokenClass(char character);
    int strlength(char *my_string);

    static int current_character = 0;
    char *input = source_text + current_character;
    int index = 0;
    enum characterclass initial_class;
    bool period_found = false;
    int identifiers_length = strlength(identifier_legal_characters);
    int delimiters_size = strlength(delimiters);
    int integers_length = strlength(integer_characters);
    int spaces_length = strlength(spaces);

    for (int i = 0; i < KEYWORD_MAX_LENGTH; i++)
        keyword_unique_length[i] = strlength(keywords[0]);

    struct found_token next_token;

    while (checkCharInRange(*(input + index), spaces, spaces_length))
    {
        current_character++;
        input++;
    }

    if (*input == '\0')
    {
        printf("End of input string reached\n");
        next_token.token_type = ERROR;
        return next_token;
    }

    initial_class = initialTokenClass(*input);
    next_token.token_start[index] = *(input + index);
    index++;

    switch (initial_class)
    {
    case CLASS_INTEGER:
        while (!checkCharInRange(*(input + index), delimiters, delimiters_size))
        {
            if (*(input + index) == period)
            {
                if (period_found)
                {
                    next_token.token_type = ERROR;
                    break;
                }
                next_token.token_type = DOUBLE;
            }

            if (checkCharInRange(*(input + index), integer_characters, integers_length))
            {
                if (!period_found)
                    next_token.token_type = INTEGER;
            }
            else
            {
                next_token.token_type = ERROR;
                break;
            }

            next_token.token_start[index] = *(input + index);
            index++;
        }

        break;
    case CLASS_ALPHABET:
        while (!checkCharInRange(*(input + index), delimiters, delimiters_size))
        {
            if (checkCharInRange(*(input + index), identifier_legal_characters, identifiers_length))
                next_token.token_type = IDENTIFIER;
            else
                next_token.token_type = ERROR;

            next_token.token_start[index] = *(input + index);
            index++;
        }

        break;
    case CLASS_KEYWORD:
        while (!checkCharInRange(*(input + index), delimiters, delimiters_size))
        {
            if (checkCharInRange(*(input + index), keywords[index], keyword_unique_length[index]))
                next_token.token_type = KEYWORD;
            else if (checkCharInRange(*(input + index), identifier_legal_characters, identifiers_length))
                next_token.token_type = IDENTIFIER;
            else
                next_token.token_type = ERROR;

            next_token.token_start[index] = *(input + index);
            index++;
        }

        break;
    case CLASS_QUOTEMARK:
        while (*(input + index) != '\n')
        {
            if (*(input + index) == '\"')
                break;

            next_token.token_start[index] = *(input + index);
            index++;
        }

        next_token.token_type = STRING;

        break;
    case CLASS_OPERATOR:
        if (!checkCharInRange(*(input + index), delimiters, delimiters_size))
            if (checkCharInRange(*(input + index), operators[index], operator_length[index]))
            {
                next_token.token_start[index] = *(input + index);
                index++;
            }

        next_token.token_type = OPERATOR;
        break;
    default:
        next_token.token_type = ERROR;
        break;
    }

    current_character += index;
    next_token.token_start[index] = '\0';
    
    next_token.token_length = index;

    return next_token;
}

enum characterclass initialTokenClass(char character)
{
    bool checkCharInRange(char character, char *range, int length);
    int strlength(char *my_string);

    int first_list = 0;
    keyword_unique_length[first_list] = strlength(keywords[first_list]);
    operator_length[first_list] = strlength(operators[first_list]);
    int identifiers_length = strlength(identifier_legal_characters);
    int integers_length = strlength(integer_characters);
    int delimiters_size = strlength(delimiters);

    if (checkCharInRange(character, integer_characters, integers_length))
        return CLASS_INTEGER;

    if (checkCharInRange(character, keywords[0], keyword_unique_length[0]))
        return CLASS_KEYWORD;

    if (checkCharInRange(character, identifier_legal_characters, identifiers_length))
        return CLASS_ALPHABET;

    if (checkCharInRange(character, operators[first_list], operator_length[first_list]))
        return CLASS_OPERATOR;

    if (character == '\"')
        return CLASS_QUOTEMARK;

    return CLASS_ERROR;
}

bool checkCharInRange(char character, char *range, int length)
{
    // check if the current character belongs to a range
    // the given range can be the ranges we create

    for (int i = 0; i < length; i++)
        if (character == range[i])
            // if it is return true
            return true;

    // if not return false
    return false;
}

int getSourceCode(char *filename, char *source_text, FILE *source_file_ptr)
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
        // printf("%c", input_symbol);
        index++;
    }
}

int strlength(char *my_string)
{
    int i = 0;
    while (*(my_string + i) != '\0')
        i++;

    return i;
}