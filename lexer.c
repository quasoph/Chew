#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

size_t line_number = 0;

Token *generate_keyword_or_identifier(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
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
        token->value = current;
    }
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_operator(char *current) {
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = OPERATOR;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_separator(char *current) {
    char *keyword = malloc(sizeof(char) * 8);
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = SEPARATOR;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_string_literal(char *current) {
    char *string_literal = malloc(sizeof(char) * 64);
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = STRING;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_int_literal(char *current) {
    char *value = malloc(sizeof(char) * 8);
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = INT;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token lexer(FILE *file) {
    
    int num_tokens = 12;
    int tokens_size = 0;
    int tokens_idx = 0;

    Token *tokens = malloc(sizeof(Token) * num_tokens);

    int current;

    while ((current = fgetc(file)) != EOF) {

        char ch = (char) current;

        Token *token = malloc(sizeof(Token));
        
        regex_t reegex;

        int operator_rgx = regcomp(&reegex, "\\(|\\)|!|%|&|;|\\*", REG_EXTENDED);
        operator_rgx = regexec(&reegex, &ch, 0, NULL, 0);
        int separator_rgx = regcomp(&reegex, "[:blank:]", REG_EXTENDED);
        separator_rgx = regexec(&reegex, &ch, 0, NULL, 0);

        if (!operator_rgx) {
            token = generate_operator(&ch);
            printf("FOUND OPERATOR %c\n", ch);
        } else if (!separator_rgx) {
            token = generate_separator(&ch);
            printf("FOUND SEPARATOR %c\n", ch);
        } else if (ch == '"') {
            int cnt = 0;
            char current_word[20];
            while ((char)(current = fgetc(file)) != '"' && cnt < 20) {
                current_word[cnt] = (char) current;
                cnt++;
            }
            token = generate_string_literal(current_word);
            printf("FOUND STRING LITERAL %s\n", current_word);
        } else if (isalpha(ch)) {
            int cnt = 0;
            char current_word[20];
            current_word[cnt] = ch;
            while (isalpha(((char)(current = fgetc(file)))) && cnt < 20) {
                current_word[cnt+1] = (char) current;
                cnt++;
            }
            token = generate_keyword_or_identifier(current_word);
            printf("FOUND KEYWORD %s\n", current_word);
        } else if (isdigit(ch)) {
            token = generate_int_literal(&ch);
            printf("FOUND INTEGER LITERAL %c\n", ch);
        }
        tokens[tokens_idx] = *token;
        tokens_idx++;
        num_tokens++;
    }
    tokens[tokens_idx].value = 0;
    tokens[tokens_idx].type = EOT;
    return *tokens;
}
