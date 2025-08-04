#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

size_t line_number = 0;

Token *generate_keyword_or_identifier(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
    keyword = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

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
    token->line_num = line_number;

    token->type = OPERATOR;
    token->value = keyword;
    return token;
}

Token *generate_separator(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
    keyword = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = SEPARATOR;
    token->value = keyword;
    return token;
}

Token *generate_string_literal(char *current) {
    char *string_literal = malloc(sizeof(char) * 64);
    string_literal = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = STRING;
    token->value = string_literal;

    return token;
}

Token *generate_int_literal(char *current) {
    char *value = malloc(sizeof(char) * 8);
    value = current;
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = INT;
    token->value = value;

    return token;
}

int printToken(Token *token) {
    printf("FOUND %s", token->value);
    return 0;
}

/* Create a generate_start, generate_end and corresponding regex for the lexer. */

Token lexer(FILE *file) {
    printf("yo");
    int num_tokens = 12;
    Token *tokens = malloc(sizeof(Token) * num_tokens);
    int tokens_idx = 0;

    char current = fgetc(file);
    while (current != EOF) {
        Token *token = malloc(sizeof(Token));
        regex_t reegex;

        int operator_rgx = regcomp(&reegex, "!|=|+|;|(|)|{|}", 0);
        operator_rgx = regexec(&reegex, &current, 0, NULL, 0);
        int separator_rgx = regcomp(&reegex, "w+", 0);
        separator_rgx = regexec(&reegex, &current, 0, NULL, 0);

        if (operator_rgx) {
            token = generate_operator(&current);
            printf("FOUND OPERATOR");
        } else if (separator_rgx) {
            token = generate_separator(&current);
            printf("FOUND SEPARATOR");
        } else if (current == '"') {
            token = generate_string_literal(&current);
            printf("FOUND STRING LITERAL");
        } else if (isalpha(current)) {
            token = generate_keyword_or_identifier(&current);
            printf("FOUND KEYWORD");
        } else if (isdigit(current)) {
            token = generate_int_literal(&current);
            printf("FOUND INTEGER LITERAL");
        }
        tokens[tokens_idx] = *token;
        tokens_idx++;
    }
    tokens[tokens_idx].value = 0;
    tokens[tokens_idx].type = EOT;
    return *tokens;
}
