#ifndef _APEX_INST_H_
#define _APEX_INST_H_
/**
 *  instruction.h
 *  Contains various Instruction-specific stage processing logic
 *
 *  Author :
 *  Harshit Vadodaria (havdoda1@binghamton.edu)
 *  State University of New York, Binghamton
 */

typedef enum {
    ADD,
    ADDL,
    SUB,
    SUBL,
    MUL,
    MOVC,
    STORE,
    STR,
    LOAD,
    LDR,
    EXOR,
    AND,
    OR,

    NOP,
    _BUBBLE,
    __NUM_INSTRUCTIONS
} opcode;

/* Format of an APEX instruction  */
typedef struct APEX_Instruction
{
  // char opcode[128];	// Operation Code
  opcode opcode;
  int rd;		    // Destination Register Address
  int rs1;		    // Source-1 Register Address
  int rs2;		    // Source-2 Register Address
  int rs3;		    // Source-2 Register Address
  int imm;		    // Literal Value
    
  // For display purpose only
  int instNum;
  char instText[128];
} APEX_Instruction;


/* Evaluates OpCode string and returns corresponding
OpCode Enum accordingly */
opcode evalOpCode(const char* opCode);

/* Evaluates Instruction's source and destination registers */
void evalInstRegVal(APEX_Instruction* ins, char tokens[]);

char* opcodeToStr(opcode opcode);

#endif
