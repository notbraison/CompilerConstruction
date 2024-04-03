#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 128  // Maximum length of a token
#define SOURCE_CODE_LIMIT 512 // Maximum length of the source code

// Token types enumeration
typedef enum
{
    IDENTIFIER, // Represents identifiers (variable names, function names, etc.)
    KEYWORD,    // Represents keywords in the programming language (if, else, while, etc.)
    INTEGER,    // Represents integer constants
    DOUBLE,     // Represents double constants
    STRING,     // Represents string literals
    OPERATOR,   // Represents operators (+, -, *, /, etc.)
    BOOLEAN,    // Represents boolean constants (true or false)
    ERROR       // Represents an error, used when a token cannot be classified
} TokenType;    // Token type enumeration

// Token structure
typedef struct
{
    TokenType type;                // Type of the token, such as keyword, identifier, operator, etc.
    char lexeme[MAX_TOKEN_LENGTH]; // The actual text representing the token
} Token;

// Function to check if a string is a keyword
int isKeyword(char *word)
{
    // Array of keywords
    char keywords[7][10] = {"if", "else", "while", "int", "double", "bool", "String"};

    // Loop through each keyword
    for (int i = 0; i < 7; i++)
    {
        // Compare the given word with each keyword
        if (strcmp(word, keywords[i]) == 0)
        {
            // If a match is found, return 1 (true)
            return 1;
        }
    }
    // If no match is found, return 0 (false)
    return 0;
}

/// Function to classify token type
TokenType classifyToken(char *lexeme)
{
    // Check if the first character of the lexeme is an alphabet or underscore (for identifiers)
    if (isalpha(lexeme[0]) || lexeme[0] == '_')
    {
        // If it's a keyword, return KEYWORD; otherwise, return IDENTIFIER
        return isKeyword(lexeme) ? KEYWORD : IDENTIFIER;
    }
    // Check if the first character is a digit (for integers or doubles)
    else if (isdigit(lexeme[0]))
    {
        // Check if the lexeme contains a '.' to classify it as a double
        if (strchr(lexeme, '.') != NULL)
        {
            return DOUBLE;
        }
        else
        {
            return INTEGER;
        }
    }
    // Check if the first character is a double quote (for strings)
    else if (lexeme[0] == '"')
    {
        return STRING;
    }
    else
    {
        // Check for operators
        char operators[18][3] = {"+", "-", "*", "/", "%%", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="};
        for (int i = 0; i < 18; i++)
        {
            // Compare the lexeme with each operator
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
        // If none of the above matches, return ERROR
        return ERROR;
    }
}

// Function to scan tokens from input
void scanTokens(char *input)
{
    char delimiters[] = " \t\n{};()";        // Delimiters used to tokenize the input string
    char *token = strtok(input, delimiters); // Tokenize the input string using strtok function
    while (token != NULL)
    {
        // Classify the token type using the classifyToken function
        TokenType type = classifyToken(token);

        // Print the token and its corresponding type
        printf("Token: %s\tType: ", token);
        switch (type)
        {
        // Print the type of token based on the TokenType enumeration
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
        token = strtok(NULL, delimiters); // Get the next token
    }
}

// Function to read the source code from a file and store it in a character array.
int get_source_code(char *filename, char *source_text)
{
    FILE *source_file_ptr; // Pointer to the source code file
    char input_symbol;     // Temp storage for characters from the file
    int index = 0;         // Loop counter
    int result = 0;        // Result

    // Attempt to open the file for reading
    if ((source_file_ptr = fopen(filename, "r")) == NULL)
    {
        // if attempt to open file for reading is null return error message
        printf("Could not open file, %s", filename);
        return result;
    }
    // If the file was successfully opened, set the result indicator to 1
    result = 1;

    // Loop through the file until the end is reached
    while ((input_symbol = fgetc(source_file_ptr)) != EOF)
    // Return the character currently pointed by the index
    // The index file position indicator is then advanced to the next character in stream.
    {
        // Store each character from the file into the source_text array
        source_text[index] = input_symbol;
        index++; // Increment the index for the next character
    }
    fclose(source_file_ptr); // close the file
    // return result;           // return result
}

// Main
int main(int argc, char **argv)
{
    char *source_file_name;              // Variable to store the name of the source file
    int CMD_args = 2;                    // Expected number of command-line arguments
    int filename_index = 1;              // Index of the filename argument in the command-line arguments array
    char source_text[SOURCE_CODE_LIMIT]; // Array to store the source code read from the file

    int get_source_code(char *filename, char *source_text); // Declaration of the function to get the source code

    // Check if the number of command-line arguments is correct
    if (argc != CMD_args)
    {
        printf("Wrong number of arguments"); // Print an error message
        exit(EXIT_FAILURE);                  // Exit the program with failure status
    }

    // Extract the source file name from the command-line arguments
    source_file_name = argv[filename_index];

    // Attempt to get the source code from the file
    if (!get_source_code(source_file_name, source_text))
        exit(EXIT_FAILURE); // If unsuccessful, exit the program with failure status

    // Print the source code
    printf("Scanning input program:\n%s\n", source_text);
    printf("==============================================\n");

    // Scan tokens from the source code and print their types
    scanTokens(source_text);

    exit(EXIT_SUCCESS); // Exit the program with success status
}
