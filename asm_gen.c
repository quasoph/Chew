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


static size_t buf_add_tabbed(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return 0;
    size_t n = 0;
    dst[0] = '\0';
    if (!src || !*src) return 0;

    #define EMIT(ch) do { if (n + 1 < cap) dst[n++] = (ch); else { dst[n] = '\0'; return n; } } while(0)

    EMIT('\t');
    for (const char *p = src; *p; ++p) {
        if (*p == '\r') {
            EMIT('\r');
            if (p[1] == '\n') continue;
        } else if (*p == '\n') {
            EMIT('\n');
            if (p[1] != '\0') EMIT('\t');
        } else {
            EMIT(*p);
        }
    }
    dst[n] = '\0';
    return n;

    #undef EMIT
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

char *global_variable_declarer(TokenList *list, char *buffer) {
    int x;
    char *buffer_at = buffer;
    buffer_at += sprintf(buffer_at, "\n.data");
    for (x=0; x<list->count; x++) {
        if (list->tokens[x].type == IDENTIFIER) {
            buffer_at += sprintf(buffer_at, "\n.globl _%s", list->tokens[x].value);
            buffer_at += sprintf(buffer_at, "\n_%s:", list->tokens[x].value);
            buffer_at += sprintf(buffer_at, "\n\t.quad 0");
        }
    }
    return buffer_at;
}

char *code_generator(ASTNode *node, char *out, char *globl, int emit) {
    if (!node) {
        return 0;
    }
    switch (node->type) {
        case TERM:
            if (node->left) {
                code_generator(node->left, out, globl, 0);
            }
            if (node->right) {
                code_generator(node->right, out, globl, 0);
            }
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
            break;
        case VAR_ASSIGN: {
            if (node->left) {
                code_generator(node->left, out, globl, 0);
            }
            if (node->right) {
                code_generator(node->right, out, globl, 0);
            }
        // variable assignment: stored in memory so far, yet to be loaded into register
            char buf[1024];
            char *at = buf;
            if (node->left && node->right) {
                at += sprintf(at, "\nldr x%d, =_%s", node->left->reg, node->left->value);
                if (node->right->token_type == STRING) {
                    globl += sprintf(globl, "\n.globl _%s", node->right->value);
                    globl += sprintf(globl, "\n_%s:", node->right->value);
                    globl += sprintf(globl, "\n\t.asciz \"%s\"", node->right->value);
                    at += sprintf(at, "\nldr x%d, =_%s", node->right->reg, node->right->value);
                } else if (node->right->token_type == INT) {
                    at += sprintf(at, "\nmov x%d, %s", node->right->reg, node->right->value);
                } else if (node->right->token_type == IDENTIFIER) {
                    at += sprintf(at, "\nldr x%d, =_%s", node->right->reg, node->right->value);
                }
                at += sprintf(at, "\nstr x%d, [x%d]", node->right->reg, node->left->reg);
                node->reg = node->left->reg;
                node->output = strdup(buf);
                scratch_free(node->left->reg);
                scratch_free(node->right->reg);
                if (emit == 1) {
                    strcat(out, buf);
                }
            }
            break;}
        case VAR_LESS_THAN:
            break;
        case VAR_GREATER_THAN:
            break;
        case IF_THEN:
            if (node->left) {
                code_generator(node->left, out, globl, 0);
            }
            if (node->right) {
                code_generator(node->right, out, globl, 0);
            }
            
            char ifbuf[1024];
            char *ifat = ifbuf;
            size_t remaining = sizeof(ifbuf);

            if (node->left && node->right) {
                ASTNode *cond = node->left;
                ASTNode *then = node->right;
                int rcond = cond->reg;
                int rthen = then->reg;

                char *label = label_name(node->reg);
                ifat += sprintf(ifat, "\n_%s:", label);
                
                ifat += snprintf(ifat, remaining, "\n");
                size_t wrote = buf_add_tabbed(ifat, remaining, then->output);
                ifat += wrote;
                remaining = (remaining > wrote) ? remaining - wrote : 0;
                
                ifat += sprintf(ifat, "\n");
                ifat += sprintf(ifat, "\ncmp %s, %s", scratch_name(rcond), cond->right->value);

                if (cond->type == VAR_ASSIGN) {
                    ifat += sprintf(ifat, "\nbeq _%s", label);
                } else if (cond->type == VAR_LESS_THAN) {
                    ifat += sprintf(ifat, "\nblt _%s", label);
                } else if (cond->type == VAR_GREATER_THAN) {
                    ifat += sprintf(ifat, "\nbgt _%s", label);
                }
                
            }
            node->output = strdup(ifbuf);
            if (emit == 1) {
                strcat(out, ifbuf);
            }
            break;
        case STATEMENT:
            if (node->left) {
                code_generator(node->left, out, globl, 0);
            }
            if (node->right) {
                code_generator(node->right, out, globl, 0);
            }
            char stmtbuf[1024];
            char *stmtbuf_at = stmtbuf;
            if (emit == 1) {
                strcat(out, node->right->output);
            }
            break;
        case ROOT:
            if (node->left) {
                code_generator(node->left, out, globl, 0);
            }
            if (node->right) {
                code_generator(node->right, out, globl, 0);
            }
            if (emit == 1) {
                strcat(out, node->right->output);
            }
            break;
        case EMPTY:
            break;
    }

    return out;
    
}