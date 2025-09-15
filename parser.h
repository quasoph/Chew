#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef enum {
    LEAF,
    IFELSE
} NodeType;

typedef struct {
    char *value;
    NodeType type;
} GenerateNode;

int i;
int acceptnonterm(Token *token, TokenType predicted);
int acceptterm(Token *token, char *predicted);
void Term(TokenList *token);
void Statement(TokenList *token);
int parser(TokenList *tokens);

#endif