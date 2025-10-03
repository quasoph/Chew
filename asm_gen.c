#include "lexer.h"
#include "parser.h"
#include "asm_gen.h"

void code_generator(ASTNode *node) {
    /* main - amend to contain updated build information */
    printf("\n	.section	__TEXT,__text,regular,pure_instructions");
	printf("\n.build_version macos, 15, 0	sdk_version 15, 4");
    printf("\n.section	__DATA,__data"); // should this be somewhere else?
    switch (node->type) {
        case 1:
        // declaration node
            printf("\ndeclaration node");
            char decl[3] = node->left->value;
        case 2:
        /* variable assignment node
        .asciz adds a null byte at the end (beginning) of each ascii string */
            printf("\n.global _%s", node->left->value);
            printf("\n_%s:", node->left->value);
            if (decl == "int") {
                printf("\n    .long %c", node->right->value);
            } else if (decl == "str") {
                printf("\n    .asciz \"%s\"", node->right->value);
            } else if (decl == NULL) {
                printf("\nYou need to declare a variable before assignment!");
            } else {
                printf("\nUnrecognized type definition. Valid types are \"int\" and \"str\".");
            }
    }
    printf("\n.subsections_via_symbols"); // should this be somewhere else?
}