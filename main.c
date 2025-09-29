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
    printf("---- STEP ONE: TOKEN GENERATION ----\n");
    TokenList *tokens = lexer(file);
    printf("\n ");
    printf("\n---- STEP TWO: BUILDING THE ABSTRACT SYNTAX TREE ----");
    printf("\nNote that printed output shows abstract nodes & children working upwards from leaves.");
    parser(tokens);
    free(tokens);
    return 0;
}