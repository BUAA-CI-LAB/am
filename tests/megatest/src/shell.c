#include <lib.h>

char buf[MAX_COMMAND_NAME_LENGTH];
char commands_history[MAX_HISTORY_COMMAND_NUMBER][MAX_COMMAND_NAME_LENGTH];
int head = 0, rear = 0;

struct command commands[] = {
    {"help", help},
	{"echo", echo},
    {"clear", clear},
	{"coremark", coremark},
	{"dhrystone", dry},
    {"uart", uart_test},
    {"xilinx intc", xilinx_intc_test},
};

void help() {
    for(int i = 0; i < (sizeof commands / sizeof commands[0]); ++i) {
        printf("%d. %s\n", i + 1, commands[i].name);
    }
}

void readline(char *buf, unsigned int n) {
	for (int i = 0; i < n; ++i) {
		buf[i] = getc();
		if(buf[i] == '\x1b' && (getc() == '\x5b')) {
			char dir = getc();
			if(dir == '\x41') {
				if(commands_history[head == 0 ? MAX_HISTORY_COMMAND_NUMBER - 1 : head - 1][0]) {
					head = head == 0 ? MAX_HISTORY_COMMAND_NUMBER - 1 : head - 1;
					for(int j = 0; j < i; ++j) putc('\b'), putc(' '), putc('\b');
					i = strlen(commands_history[head]) - 1;
					strcpy(buf, commands_history[head]);
					printf("%s", commands_history[head]);
				} else {
					buf[i] = 0;
				}
			} else if(dir == '\x42') {
				if(head != rear) {
					head = head == MAX_HISTORY_COMMAND_NUMBER - 1 ? 0 : head + 1;
					for(int j = 0; j < i; ++j) putc('\b'), putc(' '), putc('\b');
					i = strlen(commands_history[head]) - 1;
					strcpy(buf, commands_history[head]);
					printf("%s", commands_history[head]);
				} else {
					buf[i] = 0;
				}
			}
			continue;
		}
		
		if(buf[i] == '\r' || buf[i] == '\n') {
			putc('\n'), putc('\r');
			buf[i] = 0;
			return;
		}

		if(buf[i] == '\x7f') {
			if(i > 0) {
				putc('\b'), putc(' '), putc('\b');
				i -= 2;
			} else {
				i--;
			}
		} else if(buf[i] != '\r' && buf[i] != '\n') {
			putc(buf[i]);
		}
	}
	printf("line too long\n");
	while (getc() != '\r' && getc() != '\n');
	buf[0] = 0;
}

void runcmd(char *s) {
	char argbuf[MAX_COMMAND_NAME_LENGTH / MAX_ARG_LENGTH][MAX_ARG_LENGTH];
	int cur_row = 0, cur_col = 0;
	for(int i = 0; i < strlen(s); ++i) {
		if(cur_row >= MAX_COMMAND_NAME_LENGTH / MAX_ARG_LENGTH) {
			printf("ARG NUM OVERFLOW! Num of args should be under %d\n", MAX_COMMAND_NAME_LENGTH / MAX_ARG_LENGTH);
			return;
		}
		if(cur_col >= MAX_ARG_LENGTH) {
			printf("ARG LENGTH OVERFLOW! Length of an arg should be under %d\n", MAX_ARG_LENGTH);
			return;
		}
		if(s[i] == ' ') {
			if(cur_col == 0) continue;
			else argbuf[cur_row][cur_col] = 0, ++cur_row, cur_col = 0;
		} else {
			argbuf[cur_row][cur_col++] = s[i];
		}
	}
	argbuf[cur_row][cur_col] = 0;
	for(int i = 0; i < (sizeof commands / sizeof commands[0]); ++i) {
		if(!strcmp(argbuf[0], commands[i].name)) {
			commands[i].func(argbuf);
			strcpy(commands_history[rear], argbuf[0]);
			head = head == MAX_HISTORY_COMMAND_NUMBER - 1 ? 0 : head + 1;
			rear = head;
			argbuf[0][0] = 0;
			return;
		}
	}
	if(strlen(s)) {
		printf("error command!\n");
	}
}

int main(int argc, char **argv) {
    // printf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	// printf("::                                                         ::\n");
	// printf("::               EuLA BootLoader Shell 2023                ::\n");
	// printf("::                                                         ::\n");
	// printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	for(;;) {
		printf("$ ");
		readline(buf, sizeof buf);
		runcmd(buf);
    }
}
