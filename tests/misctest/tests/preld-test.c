#include "trap.h"

int main()
{
    int a = 0x1c000000;
    int b = 0xbfc00000;
    __asm__ __volatile__("preld 0, %0, 0x120" : "=r"(a) : : "memory");
    __asm__ __volatile__("preld 8, %0, 0x130" : "=r"(b) : : "memory");
    __asm__ __volatile__("preld 31, %0, 0x120" : "=r"(a) : : "memory");
    return 0;
}