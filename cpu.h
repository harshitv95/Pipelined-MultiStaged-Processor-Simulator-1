#include "instruction.h"

#ifndef _APEX_CPU_H_
#define _APEX_CPU_H_
/**
 *  cpu.h
 *  Contains various CPU and Pipeline Data structures
 *
 *  Author :
 *  Gaurav Kothari (gkothar1@binghamton.edu)
 *  State University of New York, Binghamton
 */

enum
{
  F,
  DRF,
  EX1,
  EX2,
  MEM1,
  MEM2,
  WB,
  NUM_STAGES
};

/* Model of CPU stage latch */
typedef struct CPU_Stage
{
  int pc;		    // Program Counter
  // char opcode[128];	// Operation Code
  opcode opcode;
  int rs1;		    // Source-1 Register Address
  int rs2;		    // Source-2 Register Address
  int rs3;		    // Source-3 Register Address
  int rd;		    // Destination Register Address
  int imm;		    // Literal Value
  int rs1_value;	// Source-1 Register Value
  int rs2_value;	// Source-2 Register Value
  int rs3_value;	// Source-3 Register Value
  int buffer;		// Latch to hold some value
  int mem_address;	// Computed Memory Address
  int busy;		    // Flag to indicate, stage is performing some action
  int empty;
  int stalled;		// Flag to indicate, stage is stalled
  int flushed;
} CPU_Stage;

/* Forwarding Bus to forward data and tag */
typedef struct FWD_BUS
{
  int tag;
  int data;
} FWD_BUS;

/* Flags */
typedef enum {
  ZERO_FLAG,
  JUMP_FLAG,
  HALT_FLAG,

  NUM_FLAGS
} flags;

/* Model of APEX CPU */
typedef struct APEX_CPU
{
  /* Clock cycles elasped */
  int clock;

  /* Current program counter */
  int pc;

  /* Max number of registers supported by the CPU */
  int reg_count;

  /* Integer register file */
  int regs[32];
  int regs_valid[32];

  /* Register to hold the address of the next instruction in sequence,
  while temporarily switching to the JUMPed instruction address */
  int jump_address_register;

  /* Array of %NUM_STAGES% (7) CPU_stage */
  CPU_Stage stage[NUM_STAGES];

  /* Code Memory where instructions are stored */
  APEX_Instruction* code_memory;
  int code_memory_size;
  int num_instructions;

  /* Data Memory */
  int data_memory[4000];

  /* Some stats */
  int ins_completed;

  /* 2 Forwarding Buses, 1 from EX2 and another from MEM2 */
  FWD_BUS forward[2];

  /* Flags */
  int flags[NUM_FLAGS];
  int flags_valid[NUM_FLAGS];

  int fetched_before_stall;

} APEX_CPU;


APEX_Instruction*
create_code_memory(const char* filename, int* size);

APEX_CPU*
APEX_cpu_init(const char* filename);

int
APEX_cpu_run(APEX_CPU* cpu, int numCycles);

void
APEX_cpu_stop(APEX_CPU* cpu);

int
fetch(APEX_CPU* cpu);

int
decode(APEX_CPU* cpu);

int
execute1(APEX_CPU* cpu);
int
execute2(APEX_CPU* cpu);

int
memory1(APEX_CPU* cpu);
int
memory2(APEX_CPU* cpu);

int
writeback(APEX_CPU* cpu);

void print(APEX_CPU* cpu);

CPU_Stage create_bubble();

#endif
