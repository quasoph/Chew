#ifndef clox_value_h // protection
#define clox_value_h

#include "common.h" // include std bool, def and int

typedef double Value; // define type Value

typedef struct {
    int capacity;
    int count;
    Value* values; // pointer to array "values"
} ValueArray; // define structure ValueArray that has capacity, count and values

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif