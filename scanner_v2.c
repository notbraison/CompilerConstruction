// to run the program in the terminal use the program's name followed by the name of the file containing the mini program
//  ->  .\scanner test_file.txt

// Wisdom Makokha P15/81777/2017
// BRAISON ORINA  P15/142382/2021
// ALEM CYRIL   P15/2135/2021
// FIDEL OTIENO P15/1282/2010

// Group 17 compiler construction

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_KEYWORDS 10
#define KEYWORD_MAX_LENGTH 6
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
char keywords[KEYWORD_MAX_LENGTH][NUMBER_OF_KEYWORDS] = { //
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

// prototype declarations
const int strLength(char *string);
char *tokenScanner(char *source_text, struct token_struct *next_token);
bool checkCharInRange(char character, char *range);
int getSourceCode(char *filename, char *destination_text, FILE *source_file_ptr);
enum TokenType initialTokenClass(char character);
void printTokens(struct token_struct *token_list, int token_end);

int main(int argc, char **argv)
{
    FILE *source_file_ptr;         // file pointer to the mini program file
    char *source_filename;         // filename
    int CMD_args = 2;              // expected terminal arguements
    int filename_index = 1;        // index for filaname in the cmd arguemnts array
    int token_index = 0;           // number of tokens found
    char *output_format = "%-16s"; // output format for strings in the token stream output
    int lines = 0;                 // stores the number of lines currently scanned through
    char input_symbol;
    int check_file_empty = 0;

    struct token_struct token_list[TOKEN_LIMIT]; // array of tokens

    // check for proper number of arguements
    if (argc != CMD_args)
    {
        printf("Wrong number of arguements\n");
        exit(EXIT_FAILURE);
    }

    // get filename from cmd line parameters
    source_filename = argv[filename_index];
    // source_filename = "test_file.txt";

    if ((source_file_ptr = fopen(source_filename, "r")) == NULL)
    {
        printf("Could not open file, %s", source_filename);
        exit(EXIT_FAILURE);
    }

    int size = ftell(source_file_ptr);
    // checks if the file is empty
    if (size == check_file_empty)
        printf("File is empty!\n");
    else
    {
        do
        {
            int index = 0;
            char source_text[SOURCE_CODE_LIMIT]; // string containing mini program stored here
            char *next_ptr;                      // store current point in string to begin looking for tokens
            bool stop_token_scanner = false;     // stops token scanner loop

            // gets a line from the source code or can stop at the end of the file
            while ((input_symbol = fgetc(source_file_ptr)) != '\n' && (input_symbol != EOF))
            {
                source_text[index] = input_symbol;
                index++;
            }
            source_text[index] = '\0';

            // intermediate variable stores the starting point of current line
            next_ptr = source_text;
            lines++;

            while (!stop_token_scanner)
            {
                // scan for tokens
                next_ptr = tokenScanner(next_ptr, &token_list[token_index]);

                // stop scanning once end of string reached
                if (next_ptr == NULL)
                    stop_token_scanner = true;
                else
                    token_index++;
            }

            /**
            *
            * 
            * 
            *next stage of the compiler goes here
            *
            * 
            */ 


        } while (input_symbol != EOF);

        printTokens(token_list, token_index);
        printf("Tokens found: %d\n", token_index);
        printf("Lines found: %d\n", lines);
        // printf("7\n");
    }

    fclose(source_file_ptr);
    exit(EXIT_SUCCESS);
}

// function to print the tokens in the token array token_list
void printTokens(struct token_struct *token_list, int token_end)
{
    int token_index = 0;
    char *output_format = "%-16s|"; // output format for strings in the token stream output

    printf("==============================================\n");
    printf("%-16s|%-16s|%-6s|\n", "Token", "Type", "Length");

    for (; token_index < token_end; token_index++)
    {
        printf(output_format, token_list[token_index].token_string);

        switch (token_list[token_index].token_type)
        {
        case IDENTIFIER:
            printf(output_format, "Identifier");
            break;
        case KEYWORD:
            printf(output_format, "Keyword");
            break;
        case INTEGER:
            printf(output_format, "Integer");
            break;
        case DOUBLE:
            printf(output_format, "Double");
            break;
        case STRING:
            printf(output_format, "String");
            break;
        case OPERATOR:
            printf(output_format, "Operator");
            break;
        default:
            printf(output_format, "Unknown");
            break;
        }

        printf("%6d|\n", token_list[token_index].token_length);
    }
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
// tokenScanner function gets token stores in the current index in the tokens array token_list
// next_ptr stores the point from which tokenScanner will begin scanning for tokens
// tokenScanner returns the point in the string it stopped from to continue in the next iteration
char *tokenScanner(char *source_text, struct token_struct *next_token)
{
    int index = 0;
    bool period_found = false;
    bool skip_rest = false;
    int smallest_keyword_length = 2;

    // this section ensures that spaces are ignored: " \t\n\v;"
    while (checkCharInRange(*source_text, spaces))
        source_text++;

    // stop when we encounter the null pointer in the string and
    if (*source_text == '\0')
    {
        // printf("End of input string reached\n");
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
                    next_token->token_type = ERROR;
                else
                {
                    // found period then set to DOUBLE
                    period_found = true;
                    next_token->token_type = DOUBLE;
                }
            }
            // if we get any non integer character return error
            else if (!checkCharInRange(*(source_text + index), integer_characters))
                next_token->token_type = ERROR;

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
        while ((!checkCharInRange(*(source_text + index), delimiters)))
        {
            // this sequence relies on the value being preset, if it is none of these checks will change the value again, they are there to ensure some scenarios are handled
            // check if its a legal identifier character
            if (checkCharInRange(*(source_text + index), identifier_legal_characters))
            {
                // check if the character is a valid keyword character, if so then just proceed
                if (index < KEYWORD_MAX_LENGTH && checkCharInRange(*(source_text + index), keywords[index]))
                    ;
                // if already set as identifier no need to set the value again
                else if (next_token->token_type == IDENTIFIER)
                    ;
                // if already an unrecognised character is found then just leave as error
                else if (next_token->token_type == ERROR)
                    ;
                // otherwise set the character as an identifier
                else
                    next_token->token_type = IDENTIFIER;
            }
            // if unrecognised then just set as an error
            else
                next_token->token_type = ERROR;

            // store current character in token
            next_token->token_string[index] = *(source_text + index);
            index++;
        }

        // valid keyword lengths are between 2 and 6 for now
        // can be automated to pick from the list of keywords but it's okay as it is now
        if (index < smallest_keyword_length)
            next_token->token_type = IDENTIFIER;

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
        // store the closing quotation mark
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

    // add to source_text to change the pointer to reduce input set
    source_text += index;
    next_token->token_string[index] = '\0';

    // set token length
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