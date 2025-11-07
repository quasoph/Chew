#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
// LL(1) recursive descent parser.
int i;
ASTNode root;
ASTNode currentNode;

ASTNode *NewNode(char *value, NodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->value = value;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode *AddChildren(ASTNode *parent, ASTNode *left, ASTNode *right) {
    parent->left = left;
    parent->right = right;
    return parent;
}

void DrawTree(ASTNode *root) {
    if (root == NULL) {
        return;
    }
    printf("\nNode value: %s", root->value);
    DrawTree(root->left);
    DrawTree(root->right);
}

void FreeTree(ASTNode *root) {
    if (root == NULL) {
        return;
    }
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

int expectnonterm(Token *token, TokenType predicted) {
    if (token->type == predicted) {
        return 1;
    } else {
        return 0;
    }
}

int expectterm(Token *token, char *predicted) {
    if (strcmp(token->value, predicted) == 0) {
        return 1;
    } else {
        return 0;
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

ASTNode *Term(Token *token) {
    i++;
    ASTNode *new_node = NewNode(token->value, TERM);
    new_node->token_type = token->type;
    return new_node;
}

ASTNode *Statement(TokenList *tokens) {
    if (expectnonterm(&tokens->tokens[i], IDENTIFIER)) {
        ASTNode *term1 = Term(&tokens->tokens[i]);
        if (expectterm(&tokens->tokens[i], "=")) {
            acceptterm(&tokens->tokens[i], "=");
            ASTNode *term2 = Term(&tokens->tokens[i]);
            ASTNode *varAssignNode = NewNode("VAR_ASSIGN", VAR_ASSIGN);
            varAssignNode = AddChildren(varAssignNode, term1, term2);
            return varAssignNode;
        } else if (expectterm(&tokens->tokens[i], "<")) {
            acceptterm(&tokens->tokens[i], "<");
            ASTNode *term2 = Term(&tokens->tokens[i]);
            ASTNode *varLessThanNode = NewNode("VAR_LESS_THAN", VAR_LESS_THAN);
            varLessThanNode = AddChildren(varLessThanNode, term1, term2);
            return varLessThanNode;
        
        } else if (expectterm(&tokens->tokens[i], ">")) {
            acceptterm(&tokens->tokens[i], ">");
            ASTNode *term2 = Term(&tokens->tokens[i]);
            ASTNode *varGreaterThanNode = NewNode("VAR_GREATER_THAN", VAR_GREATER_THAN);
            varGreaterThanNode = AddChildren(varGreaterThanNode, term1, term2);
            return varGreaterThanNode;
        } else {
            ASTNode *emptyNode = NewNode("EMPTY", EMPTY);
            emptyNode->num = 0;
            return emptyNode;
        }
        
    } else if (expectterm(&tokens->tokens[i], "IF")) {
        acceptterm(&tokens->tokens[i], "IF");
        ASTNode *stmt1 = Statement(tokens);
        acceptterm(&tokens->tokens[i], "THEN");
        ASTNode *stmt2 = Statement(tokens);
        ASTNode *ifThenNode = NewNode("IF_THEN", IF_THEN);
        ifThenNode = AddChildren(ifThenNode, stmt1, stmt2);
        return ifThenNode;
    } else {
        ASTNode *emptyNode = NewNode("EMPTY", EMPTY);
        emptyNode->num = 0;
        return emptyNode;
    }
}

ASTNode *parser(TokenList *tokens) {
    i=0;
    printf("\nStarting at index %d, token %s.", i, tokens->tokens[i].value);
    ASTNode *root = NewNode("ROOT", ROOT);
    while (expectterm(&tokens->tokens[i], ".") == 0) {
        ASTNode *stmt = Statement(tokens);
        root = AddChildren(root, NULL, stmt);
    }
    printf("\nFile read.");
    return root;
}