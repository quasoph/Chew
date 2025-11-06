#ifndef ASM_GEN_H
#define ASM_GEN_H

char *global_variable_declarer(TokenList *list, char *buffer);
char *code_generator(ASTNode *node, char *out, char *globl, int emit);

#endif