#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

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

Token *generate_keyword_or_identifier(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
    keyword = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));

    if(strcmp(keyword, "int") == 0){
        token->type = KEYWORD;
        token->value = "INT";
    } else if(strcmp(keyword, "while") == 0){
        token->type = KEYWORD;
        token->value = "WHILE";
    } else if(strcmp(keyword, "write") == 0) {
        token->type = KEYWORD;
        token->value = "WRITE";
    } else if(strcmp(keyword, "read") == 0) {
        token->type = KEYWORD;
        token->value = "READ";
    } else {
        token->type = IDENTIFIER;
        token->value = keyword;
    }

    return token;
}

Token *generate_operator(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
    keyword = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));

    token->type = OPERATOR;
    token->value = keyword;
    return token;
}

Token *generate_separator(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
    keyword = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));

    token->type = SEPARATOR;
    token->value = keyword;
    return token;
}

Token *generate_string_literal(char *current) {
    char *string_literal = malloc(sizeof(char) * 8);
    string_literal = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));

    token->type = STRING;
    token->value = string_literal;

    return token;
}

Token *generate_int_literal(char *current) {
    char *value = malloc(sizeof(char) * 8);
    value = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));

    token->type = INT;
    token->value = value;

    return token;
}

/* Create a generate_start, generate_end and corresponding regex for the lexer. */

Token lexer(FILE *file) {

    int num_tokens = 12;
    Token *tokens = malloc(sizeof(Token) * num_tokens);
    int tokens_idx = 0;

    char current = fgetc(file);
    while (current != EOF) {
        Token *token = malloc(sizeof(Token));
        regex_t reegex;

        int operator_rgx = regcomp(&reegex, "!|=|\+|;|\(|\)|{|}", 0);
        operator_rgx = regexec(&reegex, current, 0, NULL, 0);
        int separator_rgx = regcomp(&reegex, "\w+", 0);
        separator_rgx = regexec(&reegex, current, 0, NULL, 0);

        if (operator_rgx) {
            token = generate_operator(current);
        } else if (separator_rgx) {
            token = generate_separator(current);
        } else if (current == '"') {
            token = generate_string_literal(current);
        } else if (isalpha(current)) {
            token = generate_keyword_or_identifier(current);
        } else if (isdigit(current)) {
            token = generate_int_literal(current);
        }
        tokens[tokens_idx] = *token;
        tokens_idx++;
    }
    tokens[tokens_idx].value = '\0';
    tokens[tokens_idx].type = EOT;
    return tokens;
}


int main() {

    int ch;
    
    FILE *file = fopen("file.txt", "w");
    while ((ch = getc(file)) != EOF) {
        printf(ch);
    }

    fclose(file);
    return 0;
}