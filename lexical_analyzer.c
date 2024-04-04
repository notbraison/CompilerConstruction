#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 128
#define SOURCE_CODE_LIMIT 512
#define NUMBER_OF_KEYWORDS 7
#define KEYWORD_MAX_LENGTH 10
#define NUMBER_OF_OPERATORS 18
#define OPER_MAX_LENGTH 3

// Token types
typedef enum
{ // define sets of named integer constants, providing more meaningful names for numeric values.
    IDENTIFIER,
    KEYWORD,
    INTEGER,
    DOUBLE,
    STRING,
    OPERATOR,
    BOOLEAN,
    ERROR
} TokenType;

// Token structure
typedef struct
{ // structure Token with two members: type and lexeme, allows for conveniently referring to instances of this structure as Token.
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

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

    int getSourceCode(char *filename, char *source_text, FILE *source_file_ptr);

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

    // run the scanner and get back the results
    scan_tokens(source_text);
    fclose(source_file_ptr);
    exit(EXIT_SUCCESS);
}
