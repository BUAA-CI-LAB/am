#include <lib.h>

void echo(char s[][MAX_ARG_LENGTH]) {
	for(int i = 1; i < MAX_COMMAND_NAME_LENGTH / MAX_ARG_LENGTH; ++i) {
		if(i > 1) putc(' ');
		if(s[i][0] == 0) break;
		printf("%s", s[i]);
	}
	putc('\n');
}