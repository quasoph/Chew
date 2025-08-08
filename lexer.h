#ifndef LEXER_H
#define LEXER_H

typedef enum {
    KEYWORD,
    OPERATOR,
    SEPARATOR,
    IDENTIFIER,
    INT,
    STRING,
    EOT /* End Of Tokens */
} TokenType;

typedef struct {
    TokenType type;
    char *value;
    size_t line_num;
} Token;

Token *generate_keyword_or_identifier(char *current);
Token *generate_operator(char *current);
Token *generate_separator(char *current);
Token *generate_string_literal(char *current);
Token *generate_int_literal(char *current);
int lexer(FILE *file);

#endif