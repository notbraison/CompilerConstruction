#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_PLUS,
    TOKEN_MUL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ID,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    int value; // For ID tokens
} Token;

// Global variables
Token currentToken;
const char input[] = "(a+b)*c+d"; // Replace with your input string
int pos = 0;

// Function prototypes
void getNextToken();
void parseE();
void parseT();
void parseF();

// Helper function to check if a character is an ID
int isID(char c) {
    return isalpha(c);
}

// Main parsing function
int main() {
    currentToken.type = TOKEN_EOF; // Initialize currentToken
    getNextToken(); // Get the first token
    parseE(); // Start parsing from the top-level non-terminal
    if (currentToken.type == TOKEN_EOF) {
        printf("Parsing successful!\n");
    } else {
        printf("Error: Unexpected token.\n");
    }
    return 0;
}

// Get the next token from input
void getNextToken() {
    char c = input[pos++];
    if (c == '+') {
        currentToken.type = TOKEN_PLUS;
    } else if (c == '*') {
        currentToken.type = TOKEN_MUL;
    } else if (c == '(') {
        currentToken.type = TOKEN_LPAREN;
    } else if (c == ')') {
        currentToken.type = TOKEN_RPAREN;
    } else if (isID(c)) {
        currentToken.type = TOKEN_ID;
        currentToken.value = c;
    } else if (c == '\0') {
        currentToken.type = TOKEN_EOF;
    } else {
        printf("Error: Invalid character '%c'.\n", c);
        exit(1);
    }
}

// Parse E -> E + T | T
void parseE() {
    parseT();
    while (currentToken.type == TOKEN_PLUS) {
        getNextToken(); // Move to the next token
        parseT();
    }
}

// Parse T -> T * F | F
void parseT() {
    parseF();
    while (currentToken.type == TOKEN_MUL) {
        getNextToken(); // Move to the next token
        parseF();
    }
}

// Parse F -> ( E ) | id
void parseF() {
    if (currentToken.type == TOKEN_LPAREN) {
        getNextToken(); // Move to the next token
        parseE();
        if (currentToken.type != TOKEN_RPAREN) {
            printf("Error: Expected ')'.\n");
            exit(1);
        }
        getNextToken(); // Move to the next token after ')'
    } else if (currentToken.type == TOKEN_ID) {
        getNextToken(); // Move to the next token after recognizing an ID
    } else {
        printf("Error: Unexpected token.\n");
        exit(1);
    }
}
