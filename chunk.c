#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

// define things you can do with chunks

void initChunk(Chunk* chunk) {
    // chunk initialized with 0 count, 0 capacity and filled with null code
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    // deallocate all memory, then initChunk() to zero out the fields
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeConstant(Chunk* chunk, Value value, int line) {

    // grow capacity if there's no space

    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    // write value (constant) to chunk's constants array

    writeValueArray(&chunk->constants, value); // unsure if this duplicates what happens in addConstant

    chunk->lines[chunk->count] = line; // set line
    chunk->count++; // increment count

}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    // grow capacity if count too large
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    // set byte and increment count
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}