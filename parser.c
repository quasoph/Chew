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
    printf("\nNode value: %s", current->value);
    if (current->left != NULL) {
        printf("\nLeft child value: %s", current->left->value);
    } else {
        printf("\nNo left child.");
    }
    if (current->right != NULL) {
        printf("\nRight child value: %s", current->right->value);
    } else {
        printf("\nNo right child.");
    }
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

ASTNode Term(Token *token) {
    if (acceptnonterm(token, STRING)) {
    } else if (acceptnonterm(token, IDENTIFIER)) {
    } else if (acceptnonterm(token, INT)) {
        // arithmetic options here
    } else {
        printf("Syntax error.");
    }
    ASTNode leaf = *GenerateNode(NULL, NULL, token->value, TERM);
    return leaf;
}

ASTNode Statement(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], TYPEDEF)) {
        acceptnonterm(&tokens->tokens[i], IDENTIFIER);
        ASTNode ident = Term(&tokens->tokens[i-1]);
        acceptterm(&tokens->tokens[i], "=");
        ASTNode term = Term(&tokens->tokens[i]);
        currentNode = *GenerateNode(&ident, &term, "DECLARATION", DECL);

    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
        ASTNode ident = Term(&tokens->tokens[i-1]);
        acceptterm(&tokens->tokens[i], "=");
        ASTNode term = Term(&tokens->tokens[i]);
        currentNode = *GenerateNode(&ident, &term, "VAR_ASSIGN", VAR_ASSIGN);

    } else if (acceptterm(&tokens->tokens[i], "IF")) {
        ASTNode stmt1 = Statement(tokens);
        acceptterm(&tokens->tokens[i], "THEN");
        ASTNode stmt2 = Statement(tokens);
        currentNode = *GenerateNode(&stmt1, &stmt2, "IF_THEN", IF_THEN);

    } else {
        printf("\nNo statement found.");
    }
    DrawTree(&currentNode);
    return currentNode;
}

ASTNode Block(TokenList *tokens) {
    // for later: ensure all variable assignments are preceded by declarations
    ASTNode stmt = Statement(tokens);
    currentNode = *GenerateNode(NULL, &stmt, "BLOCK", BLOCK);
    DrawTree(&currentNode);
    return currentNode;
}

int parser(TokenList *tokens) {
    i=0;
    printf("\nStarting at index %d, token %s.", i, tokens->tokens[i].value);

    while (acceptterm(&tokens->tokens[i], ".") == 0) {
        Block(tokens);
    }
    printf("\nFile read.");
    return 0;
}