#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef enum {
    DECL,
    VAR_ASSIGN,
    IF_THEN,
    TERMINAL,
    TERM,
    STATEMENT,
    BLOCK,
    ROOT
} NodeType;

typedef struct ASTNode {
    char *value;
    struct ASTNode* left;
    struct ASTNode* right;
    NodeType type;
} ASTNode;

int i;
int acceptnonterm(Token *token, TokenType predicted);
int acceptterm(Token *token, char *predicted);
ASTNode Term(Token *token);
ASTNode Statement(TokenList *tokens);
ASTNode Block(TokenList *tokens);
int parser(TokenList *tokens);

#endif