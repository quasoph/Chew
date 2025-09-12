#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

// LL(1) recursive descent parser.
int i;

int acceptnonterm(Token *token, TokenType predicted) {
    if (token->type == predicted) {
        i++;
        return 1;
    } else {
        return 0;
    }
}

int acceptterm(Token *token, char predicted) {
    if (token->value == strdup(&predicted)) {
        i++;
        return 1;
    } else {
        return 0;
    }
}

void Term(TokenList *tokens) {
    printf("Beginning term.\n");
    if (acceptnonterm(&tokens->tokens[i], STRING)) {
    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
    } else if (acceptnonterm(&tokens->tokens[i], INT)) {
        // arithmetic options here
    } else {
        printf("Syntax error.");
    }
    // generate Term node
}

void Statement(TokenList *tokens) {
    printf("Beginning statement.\n");
    if (acceptnonterm(&tokens->tokens[i], TYPEDEF)) {
        acceptnonterm(&tokens->tokens[i], IDENTIFIER);
        acceptterm(&tokens->tokens[i], '=');
        Term(tokens);
        // generate declaration node
    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
        acceptterm(&tokens->tokens[i], '=');
        Term(tokens);
        // generate variable setting node
    } else if (acceptterm(&tokens->tokens[i], 'IF')) {
        Term(tokens);
        acceptterm(&tokens->tokens[i], 'THEN');
        Statement(tokens);
        // generate ifthen or ifelse node
    }
}

void Block(TokenList *tokens) {
    printf("Beginning block.\n");
    Statement(tokens);
    // generate Block node
}

int parser(TokenList *tokens) {
    i=0;
    printf("\nStarting at index %d, token %s.\n", i, tokens->tokens[i].value);
    Statement(tokens);
    return 0;
}
