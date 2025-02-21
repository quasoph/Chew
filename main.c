#include "common.h"
#include "chunk.h"
#include "debug.h"


int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2); // add constant value to the chunk's constant pool
    writeChunk(&chunk, OP_CONSTANT); // write opcode to the chunk (aka the constant instruction)
    writeChunk(&chunk, constant); // write constant to the chunk

    writeChunk(&chunk, OP_RETURN);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}