#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "asm_gen.h"

// set up scratch registers for ARM64
// xn, n = 0, 1, 2... to access all 64 bits

int r[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
int sp = 0;
const char *name[10] = {"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"}; // include sp somewhere else?
int inUse[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int labels = 0;

int scratch_alloc() {
    for (i=0; i<6; i++) {
        if (inUse[i] == 0) {
            inUse[i] = 1;
            return i;
        }
    }
    i = 10; // set up as error code
    return i;
}

const char * scratch_name(int r) {
    return name[r];
}

void scratch_free(int r) {
    // free a given scratch register
    inUse[r] = 0;
}

void create_label(char *value) {
    labels++;
}

char *label_name(int label) {
    char *str;
    if (asprintf(&str, ".L%d", label) == -1) {
        return NULL;
    }
    return str;
}

int frame_address(int r) {
    return r * -16;
}

int push(char *value) {
    char *str;
    // should also make this fn for pushing two vars into the same 16-bit space
    sp = sp - 16;
    return asprintf(&str, "stp %s, [sp, #-16]!", value);
}

int pop(char *value) {
    char *str;
    sp = sp + 16;
    return asprintf(&str, "ldp %s, [sp], #16", value);
}

void global_variable_declarer(TokenList *list) {
    printf("\n.data");
    int x;
    for (x=0; x<list->count; x++) {
        if (list->tokens[x].type == IDENTIFIER) {
            printf("\n.globl _%s", list->tokens[x].value);
            printf("\n_%s:", list->tokens[x].value);
            printf("\n  .quad 0");
        }
    }
}

void code_generator(ASTNode *node) {
    if (!node) {
        return;
    }
    switch (node->type) {
        case TERM:
            node->reg = scratch_alloc();
        /*
            node->reg = scratch_alloc();
            char *label = label_name(node->reg);
            printf("\n.globl _%s", label);
            printf("\n_%s:", label);
            if (node->token_type == STRING) {
                printf("\n    .asciz \"%s\"", node->value);
            } else if (node->token_type == INT) {
                printf("\n  .short %s", node->value);
            } else if (node->token_type != (STRING || INT || IDENTIFIER)) {
                printf("\nERROR: Unrecognised datatype. Valid datatypes are str and int.");
                return;
            }
            printf("\nldr %s, =%s", scratch_name(node->reg), label);
            free(label);
        */
        case VAR_ASSIGN:
        // variable assignment: stored in memory so far, yet to be loaded into register
            if (node->left && node->right) {
                printf("\n");
                printf("\nldr x%d, =_%s", node->left->reg, node->left->value);
                if (node->right->token_type == STRING) {
                    printf("\nmov x%d, \"%s\"", node->right->reg, node->right->value);
                } else if (node->right->token_type == INT) {
                    printf("\nmov x%d, %s", node->right->reg, node->right->value);
                } else if (node->right->token_type == IDENTIFIER) {
                    printf("\nldr x%d, =_%s", node->right->reg, node->right->value);
                }
                
                printf("\nstr x%d, [x%d]", node->right->reg, node->left->reg);
                node->reg = node->left->reg;
                scratch_free(node->left->reg);
                scratch_free(node->right->reg);
            }
        case IF_THEN:
            if (node->left && node->right) {
                
                
            }
        case STATEMENT:
            printf("");
        case ROOT:
            printf("");
    }
    if (node->left) {
        code_generator(node->left);
    }
    if (node->right) {
        code_generator(node->right);
    }
}