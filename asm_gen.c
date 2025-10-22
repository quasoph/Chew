#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "asm_gen.h"

// set up scratch registers for ARM64
// xn, n = 0, 1, 2... to access all 64 bits

int r[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const char *name[10] = {"sp", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"};
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

int label_name(int label) {
    char *str;
    return asprintf(&str, ".L%d", label);
}

int stack_address(int r) {
    return r * -16;
}

int push(char *value) {
    char *str;
    // should also make this fn for pushing two vars into the same 16-bit space
    return asprintf(&str, "stp %s, [sp, #-16]!", value);
}

int pop(char *value) {
    char *str;
    return asprintf(&str, "ldp %s, [sp], #16", value);
}

void code_generator(ASTNode *node) {
    if (!node) {
        return;
    }
    switch (node->type) {
        case TERM:
            node->reg = scratch_alloc();
            printf("ldr %s, %s", scratch_name(node->reg), node->value);
        case VAR_ASSIGN:
        // variable assignment: stored in memory so far, yet to be loaded into register
            code_generator(node->left);
            code_generator(node->right);
            node->reg = scratch_alloc();
            printf("\n.globl _%s", node->left->value);
            printf("\n_%s:", node->left->value);
            if (node->right->token_type == STRING) {
                printf("\n    .asciz \"%s\"", node->right->value);
            } else if (node->right->token_type == INT) {
                printf("\n  .short %s", node->right->value);
            } else {
                printf("\nERROR: Unrecognised datatype. Valid datatypes are str and int.");
                return;
            }
            scratch_free(node->reg);
        case IF_THEN:
            printf("\nif/then");
        case STATEMENT:
            printf("\nstatement");
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