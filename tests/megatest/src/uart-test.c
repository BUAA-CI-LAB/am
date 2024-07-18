#include <lib.h>

void uart_test() {
    printf("uart is working, now print 10 lines:\n");
    for (int i = 0;i < 10; i++) {
        printf("this is line %d\n", i);
    }
}