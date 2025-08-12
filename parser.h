#ifndef PARSER_H
#define PARSER_H

int MAX_SIZE;

typedef struct {
    int *arr[MAX_SIZE];
    int *top;
} Stack;