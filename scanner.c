#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_KEYWORDS 10
#define KEYWORD_MAX_LENGTH 7
#define UNIQUE_OPERATOR_NUMBER 19
#define OPER_MAX_LENGTH 3
#define SOURCE_CODE_LIMIT 1024
#define TOKEN_LIMIT 512
#define TOKEN_STRING_LIMIT 64

enum TokenType
{
    IDENTIFIER,
    KEYWORD,
    INTEGER,
    DOUBLE,
    STRING,
    OPERATOR,
    BOOLEAN,
    ERROR
};

struct token_struct
{
    char token_string[TOKEN_STRING_LIMIT];
    int token_length;
    enum TokenType token_type;
};

// Int | Double | Bool | String | If | Else | While | true | false
char keywords[KEYWORD_MAX_LENGTH][NUMBER_OF_KEYWORDS] = {
    "dswbteif",
    "othlrnfa",
    "uriostl",
    "biles",
    "lnee",
    "eg"};
char *integer_characters = "0123456789";
char *small_characters = "abcdefghijklmnopqrstuvwxyz";
char *capital_characters = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
char period = '.';
char *identifier_legal_characters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIKLMNOPQRSTUVWXYZ0123456789";
char *identifier_first_characters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIKLMNOPQRSTUVWXYZ";

//"+", "-", "*", "/", "%%", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="
char operators[OPER_MAX_LENGTH][UNIQUE_OPERATOR_NUMBER] = {
    "+-*/%%|&><=!(){}\"",
    "|&="};
char *delimiters = " \t\n\v;+-*/%%|&><=!(){}";
char *spaces = " \t\n\v;";

int keyword_unique_length[KEYWORD_MAX_LENGTH];
int operator_length[OPER_MAX_LENGTH];

const int strLength(char *string);
char *tokenScanner(char *source_text, struct token_struct *next_token);
bool checkCharInRange(char character, char *range);
int getSourceCode(char *filename, char *destination_text, FILE *source_file_ptr);
enum TokenType initialTokenClass(char character);

int main(int argc, char **argv)
{
    FILE *source_file_ptr; // file pointer to the mini program file
    char *source_filename; // filename
    int CMD_args = 2;      // expected terminal arguements
    int filename_index = 1;
    char source_text[SOURCE_CODE_LIMIT];
    int number_of_tokens;
    char *next_ptr;
    int token_index = 0;

    struct token_struct token_list[TOKEN_LIMIT];

    // check for proper number of arguements
    if (argc != CMD_args)
    {
        printf("Wrong number of arguements\n");
        exit(EXIT_FAILURE);
    }

    // get filename from cmd line parameters
    source_filename = argv[filename_index];

    // obtain the source code from a file
    if (!getSourceCode(source_filename, source_text, source_file_ptr))
        exit(EXIT_FAILURE);

    next_ptr = source_text;

    printf("Scanning input mini program:\n%s\n", source_text);
    printf("==============================================\n");
    printf("Token\t\tType\t\tLength\n");

    // // run the scanner and get back the results to store in an array
    while (token_index < TOKEN_LIMIT)
    {
        next_ptr = tokenScanner(next_ptr, &token_list[token_index]);

        // printf("6\n");
        if(next_ptr == NULL)
            break;

        printf(token_list[token_index].token_string);
        printf(" \t\t");

        switch (token_list[token_index].token_type)
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
            printf("Unknown  \t");
            break;
        }

        printf("%d\n", token_list[token_index].token_length);

        token_index++;
    }

    printf("Tokens found: %d\n", token_index);

    // printf("7\n");

    // fclose(source_file_ptr);
    exit(EXIT_SUCCESS);
}

// function to get the text from the mini program and put it in a string
int getSourceCode(char *filename, char *destination_text, FILE *source_file_ptr)
{
    char input_symbol; // temporarily store characters from the file
    int index = 0;     // loop counter
    int result = 0;    // store result of file operation

    // check if the file could be opened
    if ((source_file_ptr = fopen(filename, "r")) == NULL)
    {
        printf("Could not open file, %s", filename);
        result = -1;
        return result;
    }
    result = 1;
    // printf("1\n");

    // getting text from the file and putting in given string
    while (((input_symbol = fgetc(source_file_ptr)) != EOF) && index < SOURCE_CODE_LIMIT)
    {
        destination_text[index] = input_symbol;
        // printf("%c", input_symbol);
        index++;
    }
}

// function to get tokens from an input string
char *tokenScanner(char *source_text, struct token_struct *next_token)
{
    int index = 0;
    bool period_found = false;

    // this section ensures that spaces are ignored: " \t\n\v;"
    while (checkCharInRange(*source_text, spaces))
        source_text++;

    if (*source_text == '\0')
    {
        printf("End of input string reached\n");
        return NULL;
    }
    // printf("2\n");

    // set the initial token_type from the first character
    next_token->token_type = initialTokenClass(*source_text);
    next_token->token_string[index] = *source_text;
    index++;

    switch (next_token->token_type)
    {
    case INTEGER:
        // printf("-");
        while (!checkCharInRange(*(source_text + index), delimiters))
        {
            // check for period
            if (*(source_text + index) == period)
            {
                // period should only appear once
                if (period_found)
                {
                    next_token->token_type = ERROR;
                    break;
                }
                // found period
                period_found = true;
                next_token->token_type = DOUBLE;
            }
            // look for more integers if no periods found
            else if (checkCharInRange(*(source_text), integer_characters))
            {
                if (!period_found)
                    next_token->token_type = INTEGER;
            }
            // any other character is an error
            else
            {
                next_token->token_type = ERROR;
                break;
            }

            // store current character in token
            next_token->token_string[index] = *(source_text + index);
            index++;
        }

        break;
    case IDENTIFIER:
        // printf("--");
        while (!checkCharInRange(*(source_text + index), delimiters))
        {
            // look for non legal characters and set to error
            if (!checkCharInRange(*(source_text + index), identifier_legal_characters))
                next_token->token_type = ERROR;

            // store current character in token
            next_token->token_string[index] = *(source_text + index);
            index++;
        }

        break;
    case KEYWORD:
        // printf("---");
        while (!checkCharInRange(*(source_text + index), delimiters))
        {
            if(checkCharInRange(*(source_text + index), keywords[index]));
            // if the current character is not on the keywords[index] list then we are making an identifier
            else if (checkCharInRange(*(source_text + index), identifier_legal_characters) && !checkCharInRange(*(source_text + index), keywords[index]))
                next_token->token_type = IDENTIFIER;
            else
                next_token->token_type = ERROR;

            // store current character in token
            next_token->token_string[index] = *(source_text + index);
            index++;
        }

        break;
    case STRING:
        // printf("----");
        // accept all characters until a quotation mark appears
        while (*(source_text + index) != '\"')
        {
            // store current character in token
            next_token->token_string[index] = *(source_text + index);
            index++;
        }
        //store the closing quotation mark
        next_token->token_string[index] = *(source_text + index);
        index++;

        break;
    case OPERATOR:
        // printf("-----");
        // only one more character is acceptable
        if (checkCharInRange(*(source_text + index), operators[index]))
        {
            // store current character in token
            next_token->token_string[index] = *(source_text + index);
            index++;
        }

        break;
    default:
        // printf("-------");
        // unrecognised characters
        next_token->token_type = ERROR;
        break;
    }

    //add to source_text to change the pointer to reduce input set
    source_text += index;
    next_token->token_string[index] = '\0';

    //set token length
    next_token->token_length = index;

    return source_text;
}

// function to get an initial token class to make work easier
enum TokenType initialTokenClass(char character)
{
    int first_list = 0; // variable to access the first list in a list of lists

    // keyword
    if (checkCharInRange(character, keywords[first_list]))
        return KEYWORD;

    // integer
    if (checkCharInRange(character, integer_characters))
        return INTEGER;

    // identifier
    if (checkCharInRange(character, identifier_first_characters) && !checkCharInRange(character, keywords[first_list]))
        return IDENTIFIER;

    // string
    if (character == '\"')
        return STRING;

    // printf("3\n");

    // operator
    if (checkCharInRange(character, operators[first_list]))
        return OPERATOR;

    return ERROR; // error
}

// function to check if the current character belongs to a range
// the given range can be the ranges we create
bool checkCharInRange(char character, char *range)
{

    const int length = strLength(range);

    for (int index = 0; index < length; index++)
        if (character == range[index])
            // if it is in the range return true
            return true;

    // printf("4\n");

    // if not return false
    return false;
}

// function to calculate the length of a string
const int strLength(char *string)
{
    int index = 0;
    while (*(string + index) != '\0')
        index++;

    // printf("5\n");

    return index;
}