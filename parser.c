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
    return node;
}

ASTNode *AddChildren(ASTNode *parent, ASTNode *left, ASTNode *right) {
    parent->left = left;
    parent->right = right;
    return parent;
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
    return *NewNode(token->value, TERM);
}

ASTNode Statement(TokenList *tokens) {
    if (acceptnonterm(&tokens->tokens[i], TYPEDEF)) {
        acceptnonterm(&tokens->tokens[i], IDENTIFIER);
        ASTNode term1 = Term(&tokens[i-1]);
        acceptterm(&tokens->tokens[i], "=");
        ASTNode term2 = Term(&tokens[i]);
        ASTNode declNode = *NewNode("DECLARATION", DECL);
        *AddChildren(&declNode, &term1, &term2);
        return declNode;

    } else if (acceptnonterm(&tokens->tokens[i], IDENTIFIER)) {
        ASTNode term1 = Term(tokens[i]);
        acceptterm(&tokens->tokens[i], "=");
        ASTNode term2 = Term(tokens[i]);
        ASTNode varAssignNode = *NewNode("VAR_ASSIGN", VAR_ASSIGN);
        *AddChildren(&varAssignNode, &term1, &term2);
        return varAssignNode;

    } else if (acceptterm(&tokens->tokens[i], "IF")) {
        ASTNode stmt1 = Statement(tokens);
        acceptterm(&tokens->tokens[i], "THEN");
        ASTNode stmt2 = Statement(tokens);
        ASTNode ifThenNode = *NewNode("IF_THEN", IF_THEN);
        *AddChildren(&ifThenNode, &stmt1, &stmt2);
        return ifThenNode;

    } else {
        printf("\nNo statement found.");
    }
}

ASTNode Block(TokenList *tokens) {
    // for later: ensure all variable assignments are preceded by declarations
    ASTNode stmt = Statement(tokens);
    ASTNode stmtNode = *NewNode("STATEMENT", STMT);
    *AddChildren(&stmtNode, NULL, &stmt);
    return stmtNode;
}

ASTNode *parser(TokenList *tokens) {
    i=0;
    printf("\nStarting at index %d, token %s.", i, tokens->tokens[i].value);

    ASTNode root = *NewNode("ROOT", ROOT);
    while (acceptterm(&tokens->tokens[i], ".") == 0) {
        ASTNode block = Block(tokens);
        *AddChildren(&root, NULL, &block);
    }
    printf("\nFile read.");
    return &root;
}