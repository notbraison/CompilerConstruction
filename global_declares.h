#include <string.h>

#define MAX_TOKEN_LENGTH 128
#define SOURCE_CODE_LIMIT 512
#define NUMBER_OF_KEYWORDS 8
#define KEYWORD_MAX_LENGTH 7
#define UNIQUE_OPERATOR_NUMBER 13
#define OPER_MAX_LENGTH 3
#define FOUND_IDENTIFIERS_NO 64
#define FOUND_INTEGERS_NO 64
#define FOUND_DOUBLES_NO 64
#define FOUND_STRINGS_NO 64
#define FOUND_BOOLEANS_NO 64

enum characterclass {CLASS_ALPHABET, CLASS_INTEGER, CLASS_QUOTEMARK, CLASS_OPERATOR, CLASS_KEYWORD, CLASS_ERROR};

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

//structure to store the token gotten from the input string
//it stores where the token starts and the length of the token
struct found_token
{
    char *token_start;
    int token_length;
    TokenType token_type;
    union associated_value value;
}found_identifiers[FOUND_IDENTIFIERS_NO],
found_integers[FOUND_INTEGERS_NO],
found_doubles[FOUND_DOUBLES_NO],
found_strings[FOUND_STRINGS_NO],
found_booleans[FOUND_BOOLEANS_NO];

//union to store the values of tokens
//you can only use just 
union associated_value
{
    int constant_integer;
    double constant_double;
    char * constant_string;
    bool constant_boolean;
};

// Int | Double | Bool | String | If | Else | While
//token types descriptions
char keywords[KEYWORD_MAX_LENGTH][NUMBER_OF_KEYWORDS] = {
    "dswbei",
    "othlnf",
    "uriost",
    "bile",
    "lne",
    "eg"
};
char *integer_characters = "0123456789";
char *small_characters = "abcdefghijklmnopqrstuvwxyz";
char *capital_characters = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
char period = '.';
char underscore = '_';
char *identifier_legal_characters = strcat("_", strcat(small_characters, capital_characters));

//"+", "-", "*", "/", "%%", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="
char operators[OPER_MAX_LENGTH][UNIQUE_OPERATOR_NUMBER] = {
    "+-*/%%|&><=!",
    "|&="
};
char delimiters[] = " \t\n{}()=";

int integers_length = strlen(integer_characters);
int identifiers_length = strlen(identifier_legal_characters);
int delimiters_size = strlen(delimiters);
int keyword_max_length[KEYWORD_MAX_LENGTH];
int operator_length[OPER_MAX_LENGTH];