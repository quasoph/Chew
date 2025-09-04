#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"

int main() {
    FILE *file;
    file = fopen("testlang.tl", "r");
    TokenList *tokens = lexer(file);
    printf("Starting token value: %s", tokens->tokens[0].value);
    parser(tokens);
    free(tokens);
    return 0;
}