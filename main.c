#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

int main() {
    FILE *file;
    file = fopen("testlang.tl", "r");
    lexer(file);
    return 0;
}