#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

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
        if (strchr(lexeme, '.') != NULL) {
            return DOUBLE;
        } else {
            return INTEGER;
        }
    } else if (lexeme[0] == '"') {
        // Check if lexeme is a string
        return STRING;
    } else {
        // Check for operators and boolean constants
        char operators[18][3] = {"+", "-", "*", "/", "%", "(", ")", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="};
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
    char delimiters[] = " \t\n{}()<>=!+-*/%\"";
    char* token = strtok(input, delimiters);
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

int main() {
    char input[] = "int x = 10; int y = 20; if (x < y) { x = x + 5; } else { y = y - 5; } while (x < y) { x = x + 1; }";
    printf("Scanning input program:\n%s\n", input);
    printf("==============================================\n");
    scanTokens(input);
    return 0;
}
