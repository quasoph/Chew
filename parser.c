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

ASTNode Term(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], STRING)) {
    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
    } else if (acceptnonterm(&tokens->tokens[i], INT)) {
        // arithmetic options here
    } else {
        printf("Syntax error.");
    }
    printf("\nTerm generated.");
    leaf = GenerateNode(NULL, NULL, tokens->tokens[i-1].value, TERM);
    return leaf;
}

ASTNode Statement(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], TYPEDEF)) {
        acceptnonterm(&tokens->tokens[i], IDENTIFIER);
        acceptterm(&tokens->tokens[i], "=");
        term = Term(tokens);
        currentNode = GenerateNode("DECLARATION", DECL);
        currentNode->left = GenerateNode(tokens->tokens[i-1].value, IDENTIFIER);
        currentNode->right = term;
        return currentNode;

    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
        ident = Term(tokens);
        acceptterm(&tokens->tokens[i], "=");
        term = Term(tokens);
        currentNode = GenerateNode(ident, term, "VAR_ASSIGN", VAR_ASSIGN);
        return currentNode;

    } else if (acceptterm(&tokens->tokens[i], "IF")) {
        stmt1 = Statement(tokens);
        acceptterm(&tokens->tokens[i], "THEN");
        stmt2 = Statement(tokens);
        currentNode = GenerateNode(stmt1, stmt2, "IF_THEN", IF_THEN);
        return currentNode;

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