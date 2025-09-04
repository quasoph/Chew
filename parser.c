#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

// LL(1) recursive descent parser.
int i;

int accepttype(Token *token, TokenType predicted) {
    if (token->type == predicted) {
        i++;
        return 1;
    } else {
        return 0;
    }
}

int acceptvalue(Token *token, char predicted) {
    if (token->value == strdup(&predicted)) {
        i++;
        return 1;
    } else {
        return 0;
    }
}

int expecttype(Token *token, TokenType predicted) {
    if (accepttype(token, predicted)) {
        return 1;
    } else {
        printf("Unexpected token.");
        return 0;
    }
}

int expectvalue(Token *token, char predicted) {
    if (acceptvalue(token, predicted)) {
        return 1;
    } else {
        printf("Unexpected token.");
        return 0;
    }
}

void Term(TokenList *tokens) {
    if (accepttype(&tokens->tokens[i], STRING)) {
        printf("Accepted type STRING");
    } else if (accepttype(&tokens->tokens[i], INT)) {
        printf("Accepted type INT");
    } else if (accepttype(&tokens->tokens[i], IDENTIFIER)) {
        printf("Accepted type IDENT");
    } else {
        printf("Syntax error.");
    }
}

void Statement(TokenList *tokens) {
    if (accepttype(&tokens->tokens[i], TYPEDEF)) {
        expecttype(&tokens->tokens[i], IDENTIFIER);
        expectvalue(&tokens->tokens[i], '=');
        Term(tokens);
    }
}


int parser(TokenList *tokens) {
    i=0;
    Token token = tokens->tokens[i];
    printf("\nStarting at index %d, token %s.\n", i, token.value);
    Statement(tokens);
    return 0;
}
