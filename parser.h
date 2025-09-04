#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

int i;
int accepttype(Token *token, TokenType predicted);
int acceptvalue(Token *token, char predicted);
int expecttype(Token *token, TokenType predicted);
int expectvalue(Token *token, char predicted);
void Term(TokenList *token);
void Statement(TokenList *token);
int parser(TokenList *tokens);

#endif