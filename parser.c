#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
// LL(1) recursive descent parser.
int i;
ASTNode currentNode;

ASTNode *GenerateNode(ASTNode *left, ASTNode *right, char *value, NodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->left = left;
    node->right = right;
    node->value = value;
    node->type = type;
    return node;
}

void DrawTree(ASTNode *current) {
    printf(" Node value: %s", current->value);
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
    printf("\nTerm generated.");
    GenerateNode(NULL, NULL, tokens->tokens[i-1].value, TERM);
}

void Statement(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], TYPEDEF)) {
        acceptnonterm(&tokens->tokens[i], IDENTIFIER);
        acceptterm(&tokens->tokens[i], "=");
        Term(tokens);
        printf("\nDeclaration generated.");
        currentNode.right = GenerateNode(NULL, NULL, tokens->tokens[i-1].value, DECL);
        currentNode = *currentNode.right;
    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
        acceptterm(&tokens->tokens[i], "=");
        Term(tokens);
        printf("\nVariable assignment generated.");
        currentNode.right = GenerateNode(NULL, NULL, tokens->tokens[i-1].value, VAR_ASSIGN);
        currentNode = *currentNode.right;
    } else if (acceptterm(&tokens->tokens[i], "IF")) {
        Statement(tokens);
        acceptterm(&tokens->tokens[i], "THEN");
        Statement(tokens);
        printf("\nIf/then statement generated.");
        currentNode.right = GenerateNode(NULL, NULL, tokens->tokens[i-1].value, IF_THEN);
        currentNode = *currentNode.right;
    } else {
        printf("\nNo statement found.");
    }
    DrawTree(&currentNode);
    GenerateNode(NULL, NULL, "STATEMENT", STATEMENT);
}

void Block(TokenList *tokens) {
    // for later: ensure all variable assignments are preceded by declarations
    Statement(tokens);
    currentNode.right = GenerateNode(NULL, NULL, tokens->tokens[i-1].value, BLOCK);
    currentNode = *currentNode.right;
    DrawTree(&currentNode);
}

int parser(TokenList *tokens) {
    i=0;
    printf("\nStarting at index %d, token %s.", i, tokens->tokens[i].value);

    ASTNode root = *GenerateNode(NULL, NULL, "ROOT", ROOT);
    currentNode = root;
    DrawTree(&currentNode);

    while (acceptterm(&tokens->tokens[i], ".") == 0) {
        Block(tokens);
    }
    printf("\nFile read.");
    return 0;
}
// make an AST printing function next to use in main.c