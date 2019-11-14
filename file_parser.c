/*
 *  file_parser.c
 *  Contains functions to parse input file and create
 *  code memory, you can edit this file to add new instructions
 *
 *  Author :
 *  Gaurav Kothari (gkothar1@binghamton.edu)
 *  State University of New York, Binghamton
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"


static char* remove_char(char* str, char find){
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != find) dst++;
    }
    *dst = '\0';
}

/*
 * This function is related to parsing input file
 *
 * Note : You are not supposed to edit this function
 */
static int
get_num_from_string(char* buffer)
{
  char str[16];
  int j = 0;
  for (int i = 1; buffer[i] != '\0'; ++i) {
    str[j] = buffer[i];
    j++;
  }
  str[j] = '\0';
  return atoi(str);
}

/*
 * This function is related to parsing input file
 *
 * Note : you can edit this function to add new instructions
 */
static void
create_APEX_instruction(APEX_Instruction* ins, char* buffer, int instNum)
{
  strcpy(ins->instText, buffer);
  remove_char(ins->instText, '\n');
  char* token = strtok(buffer, ",");
  int token_num = 0;
  char tokens[6][128];
  while (token != NULL) {
    strcpy(tokens[token_num], token);
    token_num++;
    token = strtok(NULL, ",");
  }

  ins->opcode = evalOpCode( tokens[0] );
  ins->instNum = instNum;
  // ins->instText = *buffer;

  switch(ins->opcode) {
    case MOVC:
      ins->rd = get_num_from_string(tokens[1]);
      ins->imm = get_num_from_string(tokens[2]);
      break;
    case STORE:
      ins->rs1 = get_num_from_string(tokens[1]);
      ins->rs2 = get_num_from_string(tokens[2]);
      ins->imm = get_num_from_string(tokens[3]);
      break;
    case STR:
      ins->rs1 = get_num_from_string(tokens[1]);
      ins->rs2 = get_num_from_string(tokens[2]);
      ins->rs3 = get_num_from_string(tokens[3]);
      break;
    case LOAD:
    case ADDL:
    case SUBL:
      ins->rd = get_num_from_string(tokens[1]);
      ins->rs1 = get_num_from_string(tokens[2]);
      ins->imm = get_num_from_string(tokens[3]);
      break;
    case ADD:
    case SUB:
    case LDR:
    case MUL:
    case AND:
    case OR:
    case EXOR:
      ins->rd = get_num_from_string(tokens[1]);
      ins->rs1 = get_num_from_string(tokens[2]);
      ins->rs2 = get_num_from_string(tokens[3]);
      break;
  }
}

/*
 * This function is related to parsing input file
 *
 * Note : You are not supposed to edit this function
 */
APEX_Instruction*
create_code_memory(const char* filename, int* size)
{
  if (!filename) {
    return NULL;
  }

  FILE* fp = fopen(filename, "r");
  if (!fp) {
    return NULL;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t nread;
  int code_memory_size = 0;

  while ((nread = getline(&line, &len, fp)) != -1) {
    code_memory_size++;
  }
  *size = code_memory_size;
  if (!code_memory_size) {
    fclose(fp);
    return NULL;
  }

  APEX_Instruction* code_memory =
    malloc(sizeof(APEX_Instruction) * code_memory_size);
  if (!code_memory) {
    fclose(fp);
    return NULL;
  }

  rewind(fp);
  int current_instruction = 0;
  while ((nread = getline(&line, &len, fp)) != -1) {
    create_APEX_instruction(&code_memory[current_instruction], line, current_instruction);
    current_instruction++;
  }

  free(line);
  fclose(fp);
  return code_memory;
}
