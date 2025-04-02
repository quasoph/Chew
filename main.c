#include "common.h"
#include "chunk.h"
#include "debug.h"

// tests chunk-related functions. disassembles at end for all to see

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk); // initialise chunk

    int constant = addConstant(&chunk, 1.2); // add constant value to the chunk's constant pool

    writeChunk(&chunk, OP_CONSTANT, 123); // count = 0, line = 0, code[count] = OP_CONSTANT, line = 123 (?)
    writeChunk(&chunk, constant, 123); // count = 1, line = 1, code[count] = constant (1.2), line = 123 (?)

    int longconstant = addConstant(&chunk, 5.678);

    writeConstant(&chunk, longconstant, 567); // count = 2, line = 2, code[count] = longconstant (5.678), line = 567 (?)

    writeChunk(&chunk, OP_RETURN, 123); // count = 3, line = 3, code[count] = OP_RETURN, line = 123 (?)
    disassembleChunk(&chunk, "test chunk"); // disassemble chunk
    freeChunk(&chunk); // free chunk
    return 0;
}