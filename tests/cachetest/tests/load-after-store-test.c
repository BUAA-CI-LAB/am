#include "../include/trap.h"

volatile int array[100];

int main()
{
    volatile uint32_t a;
    for (int i = 0; i < 100; i++) {
        array[i] = i;
        a = array[i];
    }

    return 0;
}