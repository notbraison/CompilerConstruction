#include <stdio.h>
#include <stdlib.h>
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
    int number_of_tokens;

    if (argc != CMD_args)
    {
        printf("Wrong number of arguements");
        exit(EXIT_FAILURE);
    }

    // get the filename from command line arguements
    source_file_name = argv[filename_index];

    // obtain the source code from a file
    if (!getSourceCode(source_file_name, source_text, source_file_ptr))
        exit(EXIT_FAILURE);

    printf("Scanning input program:\n%s\n", source_text);
    printf("==============================================\n");
    printf("Token\t\t  Type\t Length\n");

    // run the scanner and get back the results to store in an array
    number_of_tokens = store_token_stream(source_text);
    // printTokenStream(number_of_tokens);

    fclose(source_file_ptr);
    exit(EXIT_SUCCESS);
}

void printTokenStream(struct found_token token)
{
    printf(token.token_string);
    printf("  \t\t");

    switch (token.token_type)
    {
    case IDENTIFIER:
        printf("Identifier  \t");
        break;
    case KEYWORD:
        printf("Keyword  \t");
        break;
    case INTEGER:
        printf("Integer  \t");
        break;
    case DOUBLE:
        printf("Double  \t");
        break;
    case STRING:
        printf("String  \t");
        break;
    case OPERATOR:
        printf("Operator  \t");
        break;
    default:
        printf("Unkown  \t");
        break;
    }

    printf("\n");
}

int store_token_stream(char *input)
{
    int token_index = 0;
    struct found_token tokenScanner(char *input);
    struct found_token current_token;

    while (true)
    {
        tokens_found[token_index] = tokenScanner(input);
        if (current_token.token_type == ERROR)
            break;
        tokens_found[token_index] = current_token;
        // printf(tokens_found[token_index].token_string);
        token_index++;
    }

    return token_index;
}

struct found_token tokenScanner(char *source_text)
{
    static int current_character = 0;
    char *input = source_text + current_character;
    int index = 0;
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

    next_token.token_type = initialTokenClass(*input);
    next_token.token_string[index] = *(input + index);
    index++;

    switch (next_token.token_type)
    {
    case INTEGER:
        while (!checkCharInRange(*(input + index), delimiters, delimiters_size))
        {
            if (*(input + index) == period)
            {
                if (period_found)
                {
                    next_token.token_type = ERROR;
                    break;
                }
                period_found = true;
                next_token.token_type = DOUBLE;
            }
            else if (checkCharInRange(*(input + index), integer_characters, integers_length))
            {
                if (!period_found)
                    next_token.token_type = INTEGER;
            }
            else if (checkCharInRange(*(input + index), operators[0], operator_length[0]))
                break;
            else
            {
                next_token.token_type = ERROR;
                break;
            }

            next_token.token_string[index] = *(input + index);
            index++;
        }

        break;
    case IDENTIFIER:
        while (!checkCharInRange(*(input + index), delimiters, delimiters_size))
        {
            if (checkCharInRange(*(input + index), identifier_legal_characters, identifiers_length))
                next_token.token_type = IDENTIFIER;
            else if (checkCharInRange(*(input + index), operators[0], operator_length[0]))
                break;
            else
                next_token.token_type = ERROR;

            next_token.token_string[index] = *(input + index);
            index++;
        }

        break;
    case KEYWORD:
        do
        {
            if (checkCharInRange(*(input + index), keywords[index], keyword_unique_length[index]))
                next_token.token_type = KEYWORD;
            else if (checkCharInRange(*(input + index), identifier_legal_characters, identifiers_length) && !checkCharInRange(*(input + index), keywords[index], keyword_unique_length[index]))
                next_token.token_type = IDENTIFIER;
            else if (checkCharInRange(*(input + index), operators[0], operator_length[0]))
                break;
            else
                next_token.token_type = ERROR;

            next_token.token_string[index] = *(input + index);
            index++;
        } while (!checkCharInRange(*(input + index), delimiters, delimiters_size));

        break;
    case STRING:
        while (*(input + index) != '\n')
        {
            if (*(input + index) == '\"')
                break;

            next_token.token_string[index] = *(input + index);
            index++;
        }

        next_token.token_type = STRING;

        break;
    case OPERATOR:
        if (!checkCharInRange(*(input + index), delimiters, delimiters_size))
            if (checkCharInRange(*(input + index), operators[index], operator_length[index]))
            {
                next_token.token_string[index] = *(input + index);
                index++;
            }

        next_token.token_type = OPERATOR;
        break;
    default:
        next_token.token_type = ERROR;
        break;
    }

    current_character += index;
    next_token.token_string[index] = '\0';

    next_token.token_length = index;

    return next_token;
}

TokenType initialTokenClass(char character)
{
    int first_list = 0;
    keyword_unique_length[first_list] = strlength(keywords[first_list]);
    operator_length[first_list] = strlength(operators[first_list]);
    int identifiers_length = strlength(identifier_legal_characters);
    int integers_length = strlength(integer_characters);
    int delimiters_size = strlength(delimiters);
    int ident_first_length = strlength(identifier_first_characters);

    if (checkCharInRange(character, integer_characters, integers_length))
        return INTEGER;

    if (checkCharInRange(character, identifier_first_characters, ident_first_length) && !checkCharInRange(character, keywords[first_list], keyword_unique_length[first_list]))
        return IDENTIFIER;

    if (checkCharInRange(character, keywords[first_list], keyword_unique_length[first_list]))
        return KEYWORD;

    if (checkCharInRange(character, operators[first_list], operator_length[first_list]))
        return OPERATOR;

    if (character == '\"')
        return STRING;

    return ERROR;
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