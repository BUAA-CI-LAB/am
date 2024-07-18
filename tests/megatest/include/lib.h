#ifndef __LIB_H__
#define __LIB_H__

#include <trap.h>

void help();
void echo();
void clear();
void uart_test();
void xilinx_intc_test();
void playmedia();
void dry();
void coremark();

#define MAX_HISTORY_COMMAND_NUMBER 10
#define MAX_COMMAND_NAME_LENGTH 256
#define MAX_ARG_LENGTH 32

struct command {
    char name[MAX_COMMAND_NAME_LENGTH];
    void(*func)();
};

extern struct command commands[];

#endif