/*
 *  cpu.c
 *  Contains APEX cpu pipeline implementation
 *
 *  Author :
 *  Gaurav Kothari (gkothar1@binghamton.edu)
 *  State University of New York, Binghamton
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

/* Set this flag to 1 to enable debug messages */
#define ENABLE_DEBUG_MESSAGES 1

CPU_Stage bubble;
// TODO: Add stages
char print_stages[7][16] = {"FETCH_____STAGE\0", "DECODE_RF_STAGE\0", "EX1_______STAGE\0", "EX2_______STAGE\0", "MEMORY1___STAGE\0", "MEMORY2___STAGE\0", "WRITEBACK_STAGE\0"};

/*
 * This function creates and initializes APEX cpu.
 *
 * Note : You are free to edit this function according to your
 * 				implementation
 */
APEX_CPU*
APEX_cpu_init(const char* filename)
{
  if (!filename) {
    return NULL;
  }

  int num_arch_registers = 16;

  APEX_CPU* cpu = malloc(sizeof(*cpu));
  if (!cpu) {
    return NULL;
  }

  /* Initialize PC, Registers and all pipeline stages */
  cpu->pc = 4000;
  cpu->reg_count = num_arch_registers;
  memset(cpu->regs, 0, sizeof(int) * num_arch_registers);
  memset(cpu->regs_valid, 1, sizeof(int) * num_arch_registers);
  memset(cpu->stage, 0, sizeof(CPU_Stage) * NUM_STAGES);
  memset(cpu->data_memory, 0, sizeof(int) * 4000);

  /* Parse input file and create code memory */
  cpu->code_memory = create_code_memory(filename, &cpu->code_memory_size);

  if (!cpu->code_memory) {
    free(cpu);
    return NULL;
  }

  if (ENABLE_DEBUG_MESSAGES) {
    fprintf(stderr,
            "APEX_CPU : Initialized APEX CPU, loaded %d instructions\n",
            cpu->code_memory_size);
    fprintf(stderr, "APEX_CPU : Printing Code Memory\n");
    printf("%-9s %-9s %-9s %-9s %-9s\n", "opcode", "rd", "rs1", "rs2", "imm");

    for (int i = 0; i < cpu->code_memory_size; ++i) {
      printf("%-9s %-9d %-9d %-9d %-9d\n",
             opcodeToStr(cpu->code_memory[i].opcode),
             cpu->code_memory[i].rd,
             cpu->code_memory[i].rs1,
             cpu->code_memory[i].rs2,
             cpu->code_memory[i].imm);
    }
  }

  /* Make all stages busy except Fetch stage, initally to start the pipeline */
  /* Make instruction in all stages EMPTY (BUBBLE) except Fetch, to start the pipeline */
  for (int i = 1; i < NUM_STAGES; ++i) {
    cpu->stage[i].busy = 1;
    cpu->stage[i].opcode = _BUBBLE;
  }

  bubble.opcode = _BUBBLE;

  return cpu;
}

/*
 * This function de-allocates APEX cpu.
 *
 * Note : You are free to edit this function according to your
 * 				implementation
 */
void
APEX_cpu_stop(APEX_CPU* cpu)
{
  free(cpu->code_memory);
  free(cpu);
}

/* Converts the PC(4000 series) into
 * array index for code memory
 *
 * Note : You are not supposed to edit this function
 *
 */
int
get_code_index(int pc)
{
  return (pc - 4000) / 4;
}
int
get_memory_index(int address) {
  return address / 4;
}

CPU_Stage create_bubble() {
  // if (!bubble) {
  //   memset(&bubble, 0, sizeof(CPU_Stage));
  //   bubble->opcode = NOP;
  // }
  return bubble;
}

// void
// print_instruction(CPU_Stage* stage)
// {
//   opcode op = stage->opcode;
//   switch(op) {
//     case STORE:
//     // case ADDL:
//     // case SUBL:
//     printf(
//       "%s,R%d,R%d,#%d ", opcodeToStr(stage->opcode), stage->rs1, stage->rs2, stage->imm);
//       break;
//     case MOVC:
//       printf("%s,R%d,#%d ", opcodeToStr(stage->opcode), stage->rd, stage->imm);
//       break; 
//     default:
//     printf(
//       "%s,R%d,R%d,#%d ", opcodeToStr(stage->opcode), stage->rd, stage->rs1, stage->rs2);
//       break;
//   }
// }

static void
print_instruction(APEX_CPU* cpu, int stageId) {
  // for (int i=0;i<NUM_STAGES;i++) {
    printf("Instruction at %s --->\t\t", print_stages[stageId]);
    CPU_Stage* stage = &cpu->stage[stageId];
    if (!stage || stage->opcode == _BUBBLE || stage->pc == 0)
      printf("EMPTY");
    else {
    APEX_Instruction* ins =
        &cpu->code_memory[get_code_index(stage->pc)];
      printf("(I%d : %d) %s",
            ins->instNum,
            stage->pc,
            ins->instText
          );
    // }
  }
  printf("\n");
}


/* Debug function which dumps the cpu stage
 * content
 *
 * Note : You are not supposed to edit this function
 *
 */
static void
print_stage_content(char* name, APEX_CPU* cpu, CPU_Stage* stage)
{
  printf("%-15s: pc(%d) ", name, stage->pc);
  // print_instruction(stage);
  printf("\n");
}

static void
display_register_contents(APEX_CPU* cpu) {
  printf(
    "\n=============== STATE OF ARCHITECTURAL REGISTER FILE ==========\n"
    );
  for (int i=0; i < cpu->reg_count; i++) {
    printf("|\tREG[%d]\t|\tValue = %d\t|\tStatus = %s\t|\n",
      i,
      cpu->regs[i],
      cpu->regs_valid[i] ? "VALID" : "INVALID"
    );
  }
}

static void
display_memory_contents(APEX_CPU* cpu) {
  printf(
    "\n============== STATE OF DATA MEMORY =============\n"
  );
  // for (int i=0;)
}

void print(APEX_CPU* cpu) {
  // printf("--------------------------------\n");
  // printf("Clock Cycle #: %d\n", cpu->clock);
  // printf("--------------------------------\n");
  
}



/*
 *  Fetch Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int
fetch(APEX_CPU* cpu)
{
  CPU_Stage* stage = &cpu->stage[F];
  int code_index = get_code_index(cpu->pc);
  if (!stage->busy && !stage->stalled) {  
    /* Store current PC in fetch latch */
    stage->pc = cpu->pc;

    /* Index into code memory using this pc and copy all instruction fields into
     * fetch latch
     */
    if (code_index >= cpu->code_memory_size)
      cpu->stage[F] = create_bubble();
    else {
      APEX_Instruction* current_ins = &cpu->code_memory[code_index];
      stage->opcode = current_ins->opcode;
      stage->rd = current_ins->rd;
      stage->rs1 = current_ins->rs1;
      stage->rs2 = current_ins->rs2;
      stage->rs3 = current_ins->rs3;
      stage->imm = current_ins->imm;
      stage->rd = current_ins->rd;

      /* Update PC for next instruction */
      cpu->pc += 4;
    }

      /* Copy data from fetch latch to decode latch*/
      cpu->stage[DRF] = cpu->stage[F];

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Fetch", cpu, stage);
    print_instruction(cpu, F);
  }
  return 0;
}


/*
Register Fetch (reading register file with 3 read ports)
*/
static void
  register_read(APEX_CPU* cpu, CPU_Stage* stage, int readS1, int readS2, int readS3) {
    if (readS1)
      stage->rs1_value = cpu->regs[stage->rs1];
    if (readS2)
      stage->rs2_value = cpu->regs[stage->rs2];
    if (readS3)
      stage->rs3_value = cpu->regs[stage->rs3];
  // if (! (cpu->regs_valid[regNum]) ) {
  //   // cpu->stage->stalled = 1;
  //   cpu->stage[DRF].stalled = 1;
  //   cpu->stage[F].stalled = 1;
  // }
  // else {
  //   cpu->stage[DRF].stalled = 0;
  //   cpu->stage[F].stalled = 0;
  //   *valueField = cpu->regs[regNum];
  // }
}

/* Checks whether the specified register is valid */
static int register_valid(int regNum, APEX_CPU* cpu) {
  return cpu->regs_valid[regNum];
}

static void change_stall_status(int stageId, APEX_CPU* cpu, int stallStatus) {
  for (int i=stageId; i>=0; i--) {
    cpu->stage[i].stalled = stallStatus;
  }
}

static int has_dependency(APEX_CPU* cpu, int stageId) {
  if (!cpu) return 0;
  CPU_Stage* stage = &cpu->stage[stageId];
  if (!stage) return 0;
  switch(stage->opcode) {
    // Instructions with 2 sources and 1 destination
    case ADD:
    case LDR:
    case SUB:
    case MUL:
    case AND:
    case OR:
    case EXOR:
      return !(
        register_valid(stage->rs1, cpu) &&
        register_valid(stage->rs2, cpu) &&
        register_valid(stage->rd, cpu)
      );
    case STORE:
      return !(
        register_valid(stage->rs1, cpu) &&
        register_valid(stage->rs2, cpu)
      );
    case LOAD:
    case ADDL:
    case SUBL:
      return !(
        register_valid(stage->rs1, cpu) &&
        register_valid(stage->rd, cpu)
      );
    case MOVC:
      return !(
        register_valid(stage->rd, cpu)
      );
    case STR:
      return !(
        register_valid(stage->rs1, cpu) &&
        register_valid(stage->rs2, cpu) &&
        register_valid(stage->rs3, cpu)
      );
  }
  return 0;
}

static void register_wite(CPU_Stage* stage, APEX_CPU* cpu) {
  int regNum = stage->rd;
  cpu->regs[regNum] = stage->buffer;
  cpu->regs_valid[regNum] = 1;
}

/*
 *  Decode Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int
decode(APEX_CPU* cpu)
{

  CPU_Stage* stage = &cpu->stage[DRF];
  if (stage->opcode != NOP && stage->opcode != _BUBBLE) {
    int has_dep = has_dependency(cpu, DRF);
    change_stall_status( DRF, cpu, has_dep );
    if (has_dep)
      cpu->stage[EX1] = create_bubble();
  }

  if (!stage->busy && !stage->stalled) {
    switch((opcode)(stage->opcode)) {
      case LOAD:
      case ADDL:
      case SUBL:
        register_read(cpu, stage, 1, 0, 0);
        break;
      case ADD:
      case SUB:
      case MUL:
      case STORE:
        register_read(cpu, stage, 1, 1, 0);
        break;
      case STR:
        register_read(cpu, stage, 1, 1, 1);
        break;
    }

    /* Copy data from decode latch to execute latch*/
    cpu->stage[EX1] = cpu->stage[DRF];

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Decode/RF", cpu, stage);
    print_instruction(cpu, DRF);
  }
  return 0;
}


void lock_register(APEX_CPU* cpu, int regNum) {
  cpu->regs_valid[regNum] = 0;
}

/* Functional Units */
static int adder(APEX_CPU* cpu, int n1, int n2) {
  int res = n1 + n2;
  if (cpu)  {
    CPU_Stage* stage = &cpu->stage[EX2];
    if (stage) stage->buffer = res;
  }
  return res;
}

static int multiplier(APEX_CPU* cpu, int n1, int n2) {
  int res = n1 * n2;
  if (cpu)  {
    CPU_Stage* stage = &cpu->stage[EX2];
    if (stage) stage->buffer = res;
  }
  return res;
}

static int logical_and(APEX_CPU* cpu, int n1, int n2) {
  int res = n1 & n2;
  if (cpu)  {
    CPU_Stage* stage = &cpu->stage[EX2];
    if (stage) stage->buffer = res;
  }
  return res;
}

static int logical_exor(APEX_CPU* cpu, int n1, int n2) {
  int res = n1 ^ n2;
  if (cpu)  {
    CPU_Stage* stage = &cpu->stage[EX2];
    if (stage) stage->buffer = res;
  }
  return res;
}

static int logical_or(APEX_CPU* cpu, int n1, int n2) {
  int res = n1 | n2;
  if (cpu)  {
    CPU_Stage* stage = &cpu->stage[EX2];
    if (stage) stage->buffer = res;
  }
  return res;
}

static void memory_access(APEX_CPU* cpu, int address, char mode) {
  switch(mode) {
    case 'r':
      cpu->stage[MEM1].buffer = cpu->data_memory[get_memory_index(address)];
      break;
    case 'w':
      cpu->data_memory[get_memory_index(address)] = cpu->stage[MEM1].buffer;
      break;
  }
}
/********************/


/*
 *  Execute Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int
execute1(APEX_CPU* cpu)
{
  CPU_Stage* stage = &cpu->stage[EX1];
  if (!stage->busy && !stage->stalled) {
    switch(stage->opcode) {
      case ADD:
      case LDR:
      case SUB:
      case MOVC:
      case ADDL:
      case LOAD:
      case SUBL:
      case MUL:
      case AND:
      case OR:
      case EXOR:
        lock_register(cpu, stage->rd);
        break;
    }

    /* Copy data from Execute latch to Memory latch*/
    cpu->stage[EX2] = cpu->stage[EX1];

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Execute", cpu, stage);
    print_instruction(cpu, EX1);
  }
  return 0;
}


int
execute2(APEX_CPU* cpu)
{
  CPU_Stage* stage = &cpu->stage[EX2];
  if (!stage->busy && !stage->stalled) {

    switch(stage->opcode) {
      case STORE:
        adder(cpu, stage->rs2_value, stage->imm);
        break;
      case STR:
        adder(cpu, stage->rs2_value, stage->rs3_value);
        break;
      case ADD:
      case LDR:
        adder(cpu, stage->rs1_value, stage->rs2_value);
        break;
      case SUB:
        adder(cpu, stage->rs1_value, -1*stage->rs2_value);
        break;
      case MOVC:
      case ADDL:
      case LOAD:
        adder(cpu, stage->rs1_value, stage->imm);
        break;
      case SUBL:
        adder(cpu, stage->rs1_value, -1*(stage->imm));
        break;
      case MUL:
        multiplier(cpu, stage->rs1_value, stage->rs2_value);
        break;
      case AND:
        logical_and(cpu, stage->rs1, stage->rs2);
        break;
      case OR:
        logical_or(cpu, stage->rs1, stage->rs2);
        break;
      case EXOR:
        logical_exor(cpu, stage->rs1, stage->rs2);
        break;
    }

    /* Copy data from Execute latch to Memory latch*/
    cpu->stage[MEM1] = cpu->stage[EX2];

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Execute", cpu, stage);
    print_instruction(cpu, EX2);
  }
  return 0;
}


/*
 *  Memory Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int
memory1(APEX_CPU* cpu)
{
  CPU_Stage* stage = &cpu->stage[MEM1];
  if (!stage->busy && !stage->stalled) {

    switch (stage->opcode)
    {
    case STORE:
      memory_access(cpu, stage->buffer, 'w');
      break;
    case LOAD:
      memory_access(cpu, stage->buffer, 'r');
      break;
    }

    /* Copy data from MEM1 latch to MEM2 latch*/
    cpu->stage[MEM2] = cpu->stage[MEM1];

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Memory", cpu, stage);
    print_instruction(cpu, MEM1);
  }
  return 0;
}
int
memory2(APEX_CPU* cpu)
{
  CPU_Stage* stage = &cpu->stage[MEM2];
  if (!stage->busy && !stage->stalled) {

    /* Copy data from MEM2 latch to WB latch*/
    cpu->stage[WB] = cpu->stage[MEM2];

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Memory", cpu, stage);
    print_instruction(cpu, MEM2);
  }
  return 0;
}

/*
 *  Writeback Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int
writeback(APEX_CPU* cpu)
{
  CPU_Stage* stage = &cpu->stage[WB];
  if (!stage->busy && !stage->stalled) {

    /* Update register file */
    switch(stage->opcode) {
      // case LOAD:
      // case MOVC:
      // case ADD:
      // case ADDL:
      // case SUB:
      // case SUBL:
      // case MUL:
      case STORE:
      case STR:
      case NOP:
      break;
      case ADD:
      case ADDL:
      case SUB:
      case SUBL:
      case MUL:
        // Setting Z Flag for Arithmetic operations
        cpu->Flag_Z = stage->buffer == 0 ? 1 : 0;
      default:
        register_wite(stage, cpu);
        break;
    }

    if (stage->opcode != _BUBBLE) cpu->ins_completed++;

  }
  if (ENABLE_DEBUG_MESSAGES) {
    // print_stage_content("Writeback", cpu, stage);
    print_instruction(cpu, WB);
  }
  return 0;
}

/*
 *  APEX CPU simulation loop
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int
APEX_cpu_run(APEX_CPU* cpu, int numCycles)
{
  printf("Executing For %d cycles", numCycles);
  while (numCycles -- > 0) {

    /* All the instructions committed, so exit */
    if (cpu->ins_completed == cpu->code_memory_size) {

      break;
    }

    // if (ENABLE_DEBUG_MESSAGES) {
    //   printf("--------------------------------\n");
    //   printf("Clock Cycle #: %d\n", cpu->clock);
    //   printf("--------------------------------\n");
    // }
    // print_instructions(cpu);
    if (ENABLE_DEBUG_MESSAGES)
      printf("\n----------------------------------- CLOCK CYCLE %d -----------------------------------\n", cpu->clock);

    writeback(cpu);
    memory2(cpu);
    memory1(cpu);
    execute2(cpu);
    execute1(cpu);
    decode(cpu);
    fetch(cpu);


    cpu->clock++;
  }
  display_register_contents(cpu);
  printf("(apex) >> Simulation Complete");

  return 0;
}