#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

// LL(1) recursive descent parser.
int i;
ASTNode GenerateNode(ASTNode *left, ASTNode *right, NodeType *type) {
    GenerateNode->left = left;
    GenerateNode->right = right;
    GenerateNode->type = type;
}

int acceptnonterm(Token *token, TokenType predicted) {
    if (token->type == predicted) {
        i++;
        return 1;
    } else {
        return 0;
    }
}

int acceptterm(Token *token, char *predicted) {
    if (strcmp(token->value, predicted) == 0) {
        i++;
        return 1;
    } else {
        return 0;
    }
}

void Term(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], STRING)) {
    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
    } else if (acceptnonterm(&tokens->tokens[i], INT)) {
        // arithmetic options here
    } else {
        printf("Syntax error.");
    }
    node->type = TERM;
}

void Statement(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], TYPEDEF)) {
        acceptnonterm(&tokens->tokens[i], IDENTIFIER);
        acceptterm(&tokens->tokens[i], "=");
        Term(tokens);
        printf("\nDeclaration generated.");
        // generate declaration node
    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
        acceptterm(&tokens->tokens[i], "=");
        Term(tokens);
        printf("\nVariable assignment generated.");
        // generate variable setting node
    } else if (acceptterm(&tokens->tokens[i], "IF")) {
        Statement(tokens);
        acceptterm(&tokens->tokens[i], "THEN");
        Statement(tokens);
        printf("\nIf/then statement generated.");
        // generate ifthen or ifelse node
    } else {
        printf("\nNo statement found.");
    }
    node->type = STATEMENT;
}

void Block(TokenList *tokens) {
    // for later: ensure all variable assignments are preceded by declarations
    Statement(tokens);
    node->type = BLOCK;
}

int parser(TokenList *tokens) {
    i=0;
    printf("\nStarting at index %d, token %s.", i, tokens->tokens[i].value);

    root = GenerateNode(NULL, NULL, ROOT);
    // GenerateNode(left->left) or something

    while (acceptterm(&tokens->tokens[i], ".") == 0) {
        Block(tokens);
    }
    printf("\nFile read.");
    return 0;
}
