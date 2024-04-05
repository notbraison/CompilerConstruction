#define MAX_TOKEN_LENGTH 128
#define SOURCE_CODE_LIMIT 512
#define NUMBER_OF_KEYWORDS 10
#define KEYWORD_MAX_LENGTH 7
#define UNIQUE_OPERATOR_NUMBER 19
#define OPER_MAX_LENGTH 3
#define FOUND_IDENTIFIERS_NO 64
#define FOUND_INTEGERS_NO 64
#define FOUND_DOUBLES_NO 64
#define FOUND_STRINGS_NO 64
#define FOUND_BOOLEANS_NO 64
#define TOKEN_LIMIT 512

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

// structure to store the token gotten from the input string
// it stores where the token starts and the length of the token
struct found_token
{
    char *token_string;
    int token_length;
    TokenType token_type;
    // union associated_value value;
} tokens_found[TOKEN_LIMIT];

// union to store the values of tokens
// you can only use just
union associated_value
{
    int constant_integer;
    double constant_double;
    char *constant_string;
    bool constant_boolean;
};

// token types descriptions
// this describes the range of valid characters for each token type

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
char underscore = '_';
char *identifier_legal_characters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIKLMNOPQRSTUVWXYZ0123456789";
char *identifier_first_characters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIKLMNOPQRSTUVWXYZ";

//"+", "-", "*", "/", "%%", "||", "&&", ">", ">=", "<", "<=", "==", "!=", "!", "="
char operators[OPER_MAX_LENGTH][UNIQUE_OPERATOR_NUMBER] = {
    "+-*/%%|&><=!(){}\"",
    "|&="};
char *delimiters = " \t\n\v;";
char *spaces = " \t\n\v;";

int keyword_unique_length[KEYWORD_MAX_LENGTH];
int operator_length[OPER_MAX_LENGTH];

bool checkCharInRange(char character, char *range, int length);
TokenType initialTokenClass(char character);
int strlength(char *my_string);
int getSourceCode(char *filename, char *source_text, FILE *source_file_ptr);
struct found_token tokenScanner(char *input);
void printToken(struct found_token token);
int store_token_stream(char *input);
