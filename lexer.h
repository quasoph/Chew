#ifndef LEXER_H
#define LEXER_H

typedef enum {
    KEYWORD,
    TYPEDEF,
    OPERATOR,
    SEPARATOR,
    IDENTIFIER,
    INT,
    STRING,
    EOT /* End Of Tokens */
} TokenType;

int MAX_SIZE;

typedef struct {
    TokenType type;
    char *value;
    size_t line_num;
} Token;

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenList;

Token *generate_keyword_or_identifier(char *current);
Token *generate_operator(char *current);
Token *generate_separator(char *current);
Token *generate_string_literal(char *current);
Token *generate_int_literal(char *current);
TokenList *lexer(FILE *file);

#endif