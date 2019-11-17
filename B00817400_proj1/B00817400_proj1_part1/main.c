/*
 *  main.c
 *
 *  Author :
 *  Gaurav Kothari (gkothar1@binghamton.edu)
 *  State University of New York, Binghamton
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"


char* cliOption(char* prompt) {
  char* input = NULL;
  unsigned int len;
  // puts(prompt);
  printf("\n%s", prompt);
  int read = getline(&input, &len, stdin);
  return input;
}

int
get_num_from_string(char* buffer)
{
  char str[16];
  int j = 0;
  for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n'; ++i) {
    str[j] = buffer[i];
    j++;
  }
  str[j] = '\0';
  return atoi(str);
}

APEX_CPU* init(char* filename, int debug) {
  APEX_CPU* cpu = APEX_cpu_init(filename, debug);
  if (!cpu) {
    fprintf(stderr, "APEX_Error : Unable to initialize CPU\n");
    exit(1);
  }
  return cpu;
}

void simulate(APEX_CPU* cpu, int numCycles) {
  APEX_cpu_run(cpu, numCycles);
}

void display(APEX_CPU* cpu, int numCycles) {
  APEX_cpu_run(cpu, numCycles);
}

void stop(APEX_CPU* cpu) {
  if (cpu) APEX_cpu_stop(cpu);
  cpu = NULL;
}

void process_input(APEX_CPU* cpu, char* input, const char* argv[]) {
  if (strcmp(input, "initialize\n") == 0 || strcmp(input, "Initialize\n") == 0) {
    cpu = init(argv[1], 0);
  }
  else {
    if (!cpu) {
      printf("CPU is not initialized");
      return;
    }
    char subst[10];
    memcpy(subst, input, 8);
    subst[8] = '\0';
    int numCycles = get_num_from_string(&input[8]);
    if (strcmp(subst, "simulate") == 0 || strcmp(subst, "Simulate") == 0) {
      simulate(cpu, numCycles);
    } else if (strcmp(input, "display\n") == 0 || strcmp(input, "Display\n") == 0) {
      display(cpu, numCycles);
    } else if (strcmp(input, "stop\n") == 0 || strcmp(input, "Stop\n") == 0) {
      stop(cpu);
    } else {
      printf("Invalid Option [%s]", input);
    }
  }
}

int
main(int argc, char const* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "APEX_Help : Usage %s <input_file>\n", argv[0]);
    exit(1);
  }

  char* input = NULL;
  APEX_CPU* cpu;
  if (argc < 4) {
    // Opens a Command Line Interface
    while (strcmp(input = cliOption("APEX> "), "exit")) {
      process_input(cpu, input, argv);
    }
  } else {
    input = argv[2];
    char* numCycles_chr = argv[3];
    if (strcmp(input, "simulate") == 0 || strcmp(input, "Simulate") == 0) {
      cpu = init(argv[1], 0);
      simulate(cpu, get_num_from_string(numCycles_chr));
    } else if (strcmp(input, "display") == 0 || strcmp(input, "Display") == 0) {
      cpu = init(argv[1], 1);
      display(cpu, get_num_from_string(numCycles_chr));
    }

    stop(cpu);
  }

  return 0;
}
