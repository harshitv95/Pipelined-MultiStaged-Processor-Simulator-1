#include "instruction.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Evaluates OpCode string and returns
corresponding OpCode Enum accordingly */
opcode evalOpCode(const char* opCode) {
    if (strcmp(opCode, "STORE") == 0) return STORE;
    if (strcmp(opCode, "LOAD") == 0) return LOAD;
    if (strcmp(opCode, "MOVC") == 0) return MOVC;
    if (strcmp(opCode, "ADD") == 0) return ADD;
    if (strcmp(opCode, "ADDL") == 0) return ADDL;
    if (strcmp(opCode, "SUB") == 0) return SUB;
    if (strcmp(opCode, "SUBL") == 0) return SUBL;
    if (strcmp(opCode, "MUL") == 0) return MUL;
    if (strcmp(opCode, "EX-OR") == 0) return EXOR;
    if (strcmp(opCode, "OR") == 0) return OR;
    if (strcmp(opCode, "AND") == 0) return AND;
    if (strcmp(opCode, "STR") == 0) return STR;
    if (strcmp(opCode, "LDR") == 0) return LDR;
    if (strcmp(opCode, "MUL") == 0) return MUL;
    if (strcmp(opCode, "JUMP") == 0) return JUMP;
    if (strcmp(opCode, "BZ") == 0) return BZ;
    if (strcmp(opCode, "BNZ") == 0) return BNZ;
    if (strcmp(opCode, "HALT") == 0 ||
        strcmp(opCode, "HALT\n") == 0 ||
        strcmp(opCode, "HALT\r\n") == 0) return HALT;
    if (strcmp(opCode, "NOP") == 0) return NOP;
    // return NULL;
    printf("[%s] is not a supported OpCode in current ISA\n", opCode);
    exit(EXIT_FAILURE);
}

char* opcodeToStr(opcode opcode) {
    switch (opcode)
    {
    case ADD    : return "ADD";
    case ADDL   : return "ADDL";
    case SUB    : return "SUB";
    case SUBL   : return "SUBL";
    case MOVC   : return "MOVC";
    case MUL    : return "MUL";
    case STORE  : return "STORE";
    case LOAD   : return "LOAD";
    case STR    : return "STR";
    case LDR    : return "LDR";
    case NOP    : return "NOP";
    case JUMP   : return "JUMP";
    case BZ     : return "BZ";
    case BNZ    : return "BNZ";
    case HALT   : return "HALT";

    case _BUBBLE: return "EMPTY";
    default: return "Instruction Not Identified";
    }
}

char* inst_to_str(APEX_Instruction* ins) {
    // char* op = opcodeToStr(ins->opcode);
    // switch( ins->opcode ) {
    //     case ADD:
    //     case SUB:
    //     case MUL:
    //     case LDR:
    //     case AND:
    //     case EXOR:
    //     case OR:

    // }
    return ins->instText;
}
