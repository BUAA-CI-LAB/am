#include "trap.h"
#include <mmio.h>
#include <eula.h>


int main()
{
    int result = 0;

    int src1 = 5, src2 = 6;

    result = newinst(src1, src2, 0b00000000000111000011010110001110); // mul.w $r14, $r12, $r13

    printf("result=%d\n", result);
    return 0;
}