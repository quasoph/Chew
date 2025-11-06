#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "asm_gen.h"

int main() {
    FILE *file;
    file = fopen("testlang.ch", "r");
    printf("---- STEP ONE: TOKEN GENERATION ----\n");
    TokenList *tokens = lexer(file);
    printf("\n ");

    printf("\n---- STEP TWO: BUILDING THE ABSTRACT SYNTAX TREE ----");
    printf("\nNote that printed output shows abstract nodes & children working upwards from leaves.");
    ASTNode *node = parser(tokens); // amend parser to produce a full AST that can be worked through by the code generator
    DrawTree(node);

    printf("\n---- STEP THREE: GENERATING ASSEMBLY CODE");
    printf("\n.section .text");
    char globl[1024];
    char obuf[1024];
    globl[0] = '\0';
    obuf[0] = '\0';

    char *globl_at = global_variable_declarer(tokens, globl);
    char *overall_buf = obuf;
    char *nout = code_generator(node, overall_buf, globl_at, 1);
	printf("%s", globl);
    printf("\n\n.globl _start");
    printf("\n\n_start:");
    printf("%s", nout);
    FreeTree(node);
    free(tokens);
    return 0;
}