#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "asm_gen.h"

void code_generator(ASTNode *node) {
    /* main - amend to contain updated build information */
    printf("\n.section .text");
	printf("\n  .global _start");
    printf("\n\n_start:");
    switch (node->type) {
        case 0:
        // declaration node
            printf("\ndeclaration node");
            char decl = *node->left->value;
        case 1:
        /* variable assignment node
        .asciz adds a null byte at the end (beginning) of each ascii string */
            printf("\n.global _%s", node->left->value);
            printf("\n_%s:", node->left->value);
            if (strcmp(&decl, "int") == 0) {
                printf("\n    .long %s", node->right->value);
            } else if (strcmp(&decl, "str") == 0) {
                printf("\n    .asciz \"%s\"", node->right->value);
            } else if (strcmp(&decl, NULL) == 0) {
                printf("\nYou need to declare a variable before assignment!");
            } else {
                printf("\nUnrecognized type definition. Valid types are \"int\" and \"str\".");
            }
        case 2:
            printf("if/then");
        case 3:
            printf("terminal");
        case 4:
            printf("term");
        case 5:
            printf("statement");
        case 6:
            printf("block");
        case 7:
            printf("root");
    }
    printf("\n.subsections_via_symbols"); // should this be somewhere else?
}