#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

typedef enum {
    BOOL,
    STRING,
    INT,
    FUNC
} DeclarationType;

struct Declaration {
    char *name;
    DeclarationType *type;
    char *value;
    char *code;
    char *next;
};

typedef enum {
    STMT_DECL,
    STMT_EXPR,
    STMT_IF_ELSE,
    STMT_FOR,
    STMT_PRINT,
    STMT_RETURN,
    STMT_BLOCK
} StatementType;

struct Statement {
    StatementType kind;
    struct decl *decl;
    struct expr *init_expr;
    struct expr *expr;
    struct expr *next_expr;
    struct stmt *body;
    struct stmt *else_body;
    struct stmt *next;
};

