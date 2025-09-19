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

typedef struct {
    union {
        int value;
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
        } children;
    };
    NodeType type;
} ASTNode;

int i;
int acceptnonterm(Token *token, TokenType predicted);
int acceptterm(Token *token, char *predicted);
void Term(TokenList *token);
void Statement(TokenList *token);
int parser(TokenList *tokens);

#endif