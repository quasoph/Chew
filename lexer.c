#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

size_t line_number = 0;

Token *generate_keyword_or_identifier(char *current) {
    /*
    Token generator for keywords and identifiers.

        Parameters
        ----------
        char *current : accepts value stored in pointer for currently read
            char from file.

        Outputs
        ----------
        Token token : token with type, value and line_num.
    */
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    if(strcmp(current, "int") == 0){
        token->type = KEYWORD;
        token->value = strdup("INT");
    } else if(strcmp(current, "while") == 0){
        token->type = KEYWORD;
        token->value = strdup("WHILE");
    } else if(strcmp(current, "write") == 0) {
        token->type = KEYWORD;
        token->value = strdup("WRITE");
    } else if(strcmp(current, "read") == 0) {
        token->type = KEYWORD;
        token->value = strdup("READ");
    } else {
        token->type = IDENTIFIER;
        token->value = strdup(current);
    }
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_operator(char *current) {
    /* Token generator for operators. */
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    token->type = OPERATOR;
    token->value = strdup(current);
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_separator(char *current) {
    /* Token generator for separators. */
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    token->type = SEPARATOR;
    token->value = strdup(current);
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_string_literal(char *current) {
    /* Token generator for strings. */
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    token->type = STRING;
    token->value = strdup(current);
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_int_literal(char *current) {
    /* Token generator for integers. */
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    token->type = INT;
    token->value = strdup(current);
    printf("Token generated: %s\n", token->value);
    return token;
}

Token lexer(FILE *file) {
    /*
    Regex lexer.
        Parameters
        ----------
        FILE *file : file containing code for lexing.

        Outputs
        ----------
        TokenList *tokens : set of tokens generated.
    */
    int num_tokens = 12;
    int tokens_size = 0;
    int capacity = 12;

    Token *tokens = malloc(capacity * sizeof(Token));

    int current;
    regex_t op_reegex;
    regex_t sep_reegex;
    int operator_rgx = regcomp(&op_reegex, "+|=|-|/|!|\\*", REG_EXTENDED);
    int separator_rgx = regcomp(&sep_reegex, ";|\\(|\\)|\\{|\\}", REG_EXTENDED);

    while ((current = fgetc(file)) != EOF) {

        char ch = (char) current;
        char str[2] = {ch, '\0'};
        Token *token = malloc(sizeof(Token));
        
        operator_rgx = regexec(&op_reegex, str, 0, NULL, 0);
        separator_rgx = regexec(&sep_reegex, str, 0, NULL, 0);

        if (!operator_rgx) {
            token = generate_operator(str);
            printf("FOUND OPERATOR %c\n", ch);
        
        } else if (!separator_rgx) {
            token = generate_separator(str);
            printf("FOUND SEPARATOR %c\n", ch);
        
        } else if (ch == '"') {
            int cnt = 0;
            int cw_capacity = 16;
            char *current_word = malloc(cw_capacity * sizeof(char));
            while ((char)(current = fgetc(file)) != '"') {
                if (cnt >= cw_capacity) {
                    cw_capacity *= 2;
                    current_word = realloc(current_word, cw_capacity * sizeof(char));
                }
                current_word[cnt] = (char) current;
                cnt++;
                current_word[cnt] = '\0';
            }
            token = generate_string_literal(current_word);
            printf("FOUND STRING LITERAL %s\n", current_word);
            free(current_word);

        } else if (isalpha(ch)) {
            int cnt = 0;
            int cw_capacity = 16;
            char *current_word = malloc(cw_capacity * sizeof(char));
            current_word[cnt] = ch;
            while (isalpha(current = fgetc(file))) {
                if (cnt >= cw_capacity) {
                    cw_capacity *= 2;
                    current_word = realloc(current_word, cw_capacity * sizeof(char));
                }
                current_word[cnt+1] = (char) current;
                cnt++;
                current_word[cnt+1] = '\0';
            }
            ungetc(current, file);
            token = generate_keyword_or_identifier(current_word);
            printf("FOUND KEYWORD %s\n", current_word);
            free(current_word);

        } else if (isdigit(ch)) {
            token = generate_int_literal(str);
            printf("FOUND INTEGER LITERAL %c\n", ch);
        }
        tokens[num_tokens] = *token;
        num_tokens++;
        if (num_tokens >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
        }
        free(token);
    }
    regfree(&op_reegex);
    regfree(&sep_reegex);
    return *tokens;
}
