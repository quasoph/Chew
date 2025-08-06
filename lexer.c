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
        token->value = current;
    }
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_operator(char *current) {
    /* Token generator for operators. */
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = OPERATOR;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_separator(char *current) {
    /* Token generator for separators. */
    char *keyword = malloc(sizeof(char) * 8);
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = SEPARATOR;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_string_literal(char *current) {
    /* Token generator for strings. */
    char *string_literal = malloc(sizeof(char) * 64);
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = STRING;
    token->value = current;
    printf("Token generated: %s\n", token->value);
    return token;
}

Token *generate_int_literal(char *current) {
    /* Token generator for integers. */
    char *value = malloc(sizeof(char) * 8);
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;

    token->type = INT;
    token->value = current;
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
        Token *tokens : set of tokens generated.
    */
    int num_tokens = 0;
    int tokens_size = 0;
    int tokens_idx = 0;

    Token *tokens = malloc(sizeof(Token) * num_tokens);

    int current;
    regex_t op_reegex;
    regex_t sep_reegex;
    int operator_rgx = regcomp(&op_reegex, "\\(|\\)|!|%|&|;|\\*", REG_EXTENDED);
    int separator_rgx = regcomp(&sep_reegex, "[:blank:]", REG_EXTENDED);

    while ((current = fgetc(file)) != EOF) {

        char ch = (char) current;
        Token *token = malloc(sizeof(Token));
        
        operator_rgx = regexec(&op_reegex, &ch, 0, NULL, 0);
        separator_rgx = regexec(&sep_reegex, &ch, 0, NULL, 0);

        if (!operator_rgx) {
            token = generate_operator(&ch);
            printf("FOUND OPERATOR %c\n", ch);
            num_tokens++;
            tokens[tokens_idx] = *token;
            tokens_idx++;
        } else if (!separator_rgx) {
            token = generate_separator(&ch);
            printf("FOUND SEPARATOR %c\n", ch);
            num_tokens++;
            tokens[tokens_idx] = *token;
            tokens_idx++;
        } else if (ch == '"') {
            int cnt = 0;
            char current_word[20];
            while ((char)(current = fgetc(file)) != '"' && cnt < 20) {
                current_word[cnt] = (char) current;
                cnt++;
            }
            token = generate_string_literal(current_word);
            printf("FOUND STRING LITERAL %s\n", current_word);
            num_tokens++;
            tokens[tokens_idx] = *token;
            tokens_idx++;
        } else if (isalpha(ch)) {
            int cnt = 0;
            char current_word[20];
            current_word[cnt] = ch;
            while (isalpha(((current = fgetc(file)))) && cnt < 15) {
                current_word[cnt+1] = (char) current;
                cnt++; //need a line so that on the final character of the word, cnt++ does not happen
            }
            token = generate_keyword_or_identifier(current_word);
            printf("FOUND KEYWORD %s\n", current_word);
            num_tokens++;
            tokens[tokens_idx] = *token;
            tokens_idx++;
        } else if (isdigit(ch)) {
            token = generate_int_literal(&ch);
            printf("FOUND INTEGER LITERAL %c\n", ch);
            num_tokens++;
            tokens[tokens_idx] = *token;
            tokens_idx++;
        }
    }
    regfree(&op_reegex);
    regfree(&sep_reegex);
    return *tokens;
}
