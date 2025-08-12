#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

// LL(1) parser.

Stack nonterms = {"E", "E'", "T", "T'", "F"};

int push(Stack *stack, int value) {
    if (stack->top == MAX_SIZE) {
        fprintf(stderr, "Stack overflow.\n");
    } else {
        stack->arr[stack->top] = value;
        printf("Top is now %d.\n", value);
    }
    return stack->top;
}

int pop(Stack *stack) {
    if (stack->top == 0) {
        fprintf(stderr, "Stack underflow.\n");
    } else {
        printf("%d removed from top.\n", stack->top);
        stack->top--;
        printf("Top is now %d.\n", stack->top);
    }
    return stack->top;
}

int Parse(Token *tokens) {
    int i;
    for (i=0; i < num_tokens - 1; i++) {
        curr = tokens[i];
        next = tokens[i+1];
    }
    return 0;
}
