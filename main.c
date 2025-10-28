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
    file = fopen("testlang.tl", "r");
    printf("---- STEP ONE: TOKEN GENERATION ----\n");
    TokenList *tokens = lexer(file);
    printf("\n ");
    printf("\n---- STEP TWO: BUILDING THE ABSTRACT SYNTAX TREE ----");
    printf("\nNote that printed output shows abstract nodes & children working upwards from leaves.");
    ASTNode *node = parser(tokens); // amend parser to produce a full AST that can be worked through by the code generator
    DrawTree(node);
    printf("\n---- STEP THREE: GENERATING ASSEMBLY CODE");
    printf("\n.section .text");
    global_variable_declarer(tokens);
	printf("\n.global _start");
    printf("\n\n_start:");
    code_generator(node);
    FreeTree(node);
    free(tokens);
    return 0;
}