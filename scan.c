#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 128
#define SOURCE_CODE_LIMIT 512

// Token types
typedef enum { //define sets of named integer constants, providing more meaningful names for numeric values.
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
typedef struct { //structure Token with two members: type and lexeme, allows for conveniently referring to instances of this structure as Token.
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

// Function to check if a string is a keyword
int isKeyword(char* word) {
    char keywords[7][10] = {"if", "else", "while", "int", "double", "bool", "String"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(word, keywords[i]) == 0) { //used for comparing two strings
            return 1;
        }
    }
    return 0;
}

// Function to classify token type
TokenType classifyToken(char* lexeme) {
    if (isalpha(lexeme[0]) || lexeme[0] == '_') {
        return isKeyword(lexeme) ? KEYWORD : IDENTIFIER;
    } else if (isdigit(lexeme[0])) {
        // Check if lexeme is a double constant
        if (strchr(lexeme, '.') != NULL) {//strchr function in C is used to search for the first occurrence of a specified character in a given string
            return DOUBLE;
        } else {
            return INTEGER;
        }
    } else if (lexeme[0] == '"') {
        // Check if lexeme is a string
        return STRING;
    } else {
        // Check for operators and boolean constants
        char operators[18][3] = {"+", "-", "*", "/", "%%", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="};
        for (int i = 0; i < 18; i++) {
            if (strcmp(lexeme, operators[i]) == 0) {
                return OPERATOR;
            }
        }
        // Check for boolean constants
        if (strcmp(lexeme, "true") == 0 || strcmp(lexeme, "false") == 0) {
            return BOOLEAN;
        }
        return ERROR;
    }
}

// Function to scan tokens from input
void scanTokens(char* input) {
    char delimiters[] = " \t\n{};()";
    char* token = strtok(input, delimiters);//strtok function in C is used to tokenize (i.e., break down) a string into smaller strings
    while (token != NULL) {
        TokenType type = classifyToken(token);
        printf("Token: %s\tType: ", token);
        switch (type) {
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

int get_source_code(char *filename, char* source_text)
{
    FILE* source_file_ptr;      //file pointer to the source code file
    char input_symbol;          //temporary storage for characters from the file
    int index = 0;              //loop counter
    int result = 0;
     
    if((source_file_ptr = fopen(filename, "r")) == NULL) 
    {
        printf("Could not open file, %s", filename);
        return result;
    }
    result = 1;

    while((input_symbol = fgetc(source_file_ptr)) != EOF)
    {
        source_text[index] = input_symbol;
        index++;
    }
}

int main(int argc, char **argv) {
    char* source_file_name;
    int CMD_args = 2;
    int filename_index = 1;
    char source_text[SOURCE_CODE_LIMIT];

    int get_source_code(char *filename, char *source_text);

    if(argc != CMD_args)
    {
        printf("Wrong number of arguements");
        exit(EXIT_FAILURE);
    }

    source_file_name = argv[filename_index];
    if(!get_source_code(source_file_name, source_text))
        exit(EXIT_FAILURE);

    printf("Scanning input program:\n%s\n", source_text);
    printf("==============================================\n");
    scanTokens(source_text);
    exit(EXIT_SUCCESS);
}
