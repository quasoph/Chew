#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef enum {
    VAR_ASSIGN,
    IF_THEN,
    TERM,
    STATEMENT,
    ROOT
} NodeType;

typedef struct ASTNode {
    union {
        int num;
        struct {
            char *value;
            struct ASTNode* left;
            struct ASTNode* right;
            NodeType type;
            int reg;
            char *output;
            TokenType token_type;
        };
    };
} ASTNode;

int i;
int acceptnonterm(Token *token, TokenType predicted);
int acceptterm(Token *token, char *predicted);
ASTNode *NewNode(char *value, NodeType type);
ASTNode *AddChildren(ASTNode *parent, ASTNode *left, ASTNode *right);
ASTNode *Term(Token *token);
ASTNode *Statement(TokenList *tokens);
ASTNode *Block(TokenList *tokens);
ASTNode *parser(TokenList *tokens);
void DrawTree(ASTNode *root);
void FreeTree(ASTNode *root);

#endif