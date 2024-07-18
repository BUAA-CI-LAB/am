#include <klib.h>

#define MATRIX_SIZE 16 // Adjust this value based on the cache size and data type
unsigned long jpeg_size = 18736;
int *B = (int*)0x88000400;
int *A = (int*)0x88000000;
int *C = (int*)0x88000800;
int *D = (int*)0x88000C00;

void map_to(void* kva) {
    printf("start to map %p to 0x10000000\n", kva);
    uint32_t pa = ((uint32_t)kva) & 0x1ffff000;
    uint32_t tlb_idx = 0x12 << 24;
    uint32_t asid = 0x0;
    uint32_t tlb_entry_hi  = 0xf0000000;
    uint32_t tlb_entry_lo0 = 0x3 | (0x3 << 2) | (0x1 << 4) | (0x1 << 6) | pa >> 4;
    uint32_t tlb_entry_lo1 = 0x3 | (0x3 << 2) | (0x1 << 4) | (0x1 << 6) | pa >> 4;
	asm volatile(
        "ori $r12, %0, 0\n"
        "csrwr $r12, 0x10\n"
        "ori $r12, %1, 0\n"
	    "csrwr $r12, 0x11\n"
        "ori $r12, %2, 0\n"
	    "csrwr $r12, 0x12\n"
        "ori $r12, %3, 0\n"
	    "csrwr $r12, 0x13\n"
        "ori $r12, %4, 0\n"
	    "csrwr $r12, 0x18\n"
        "ori $r12, $r0, 0\n"
        "tlbfill" :: "r"(tlb_idx), "r"(tlb_entry_hi), "r"(tlb_entry_lo0), "r"(tlb_entry_lo1), "r"(asid)
    );
    return;
}

// Function to perform matrix multiplication
void matrix_multiply(int *A, int *B, int *C) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            C[i * MATRIX_SIZE + j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                C[i * MATRIX_SIZE + j] += A[i * MATRIX_SIZE + j] * B[k * MATRIX_SIZE + j];
            }
        }
    }
}

struct decode_ip_ctl
{
    volatile uint32_t ctrl;
    volatile uint32_t status;
    volatile uint32_t src;
    volatile uint32_t dst;
    volatile uint32_t stride;
    volatile uint32_t iocen;
};
static struct decode_ip_ctl *decode_ctl = (void*) 0x1d0a0000;

void jpeg_start_decode(void* kva, size_t filesize) {

    decode_ctl->ctrl = 0x40000000;
    decode_ctl->src = ((uint32_t)kva) & 0x1fffffff;
    decode_ctl->dst = 0x0F000000;
    decode_ctl->stride = 320 * 3; // FIXED 320x180p @ 24bits.
    decode_ctl->iocen = 1 | (1 << 1); // 打开中断输出，清理旧的中断
    decode_ctl->ctrl  = 0x80000000 | filesize;
    return;
}

int main() {
    printf("Matrix multiplication\n");
    map_to(A);
    A[0] = 1234;
    if((*(int*)0xf0000000) != 1234) {
        printf("TLB NOT WORKING!\n");
        return 1;
    }

    printf("JPEG FILESIZE IS %d kb.\n", jpeg_size / 1024);
    jpeg_start_decode((void*)0x02000000, jpeg_size);
    // Initialize matrices A and B with random values
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            A[i * MATRIX_SIZE + j] = rand();
            B[i * MATRIX_SIZE + j] = rand();
        }
    }

    // Warm-up round to bring data into cache
    printf("Warm-up round\n");
    matrix_multiply(A, B, C);

    // start JPEG Decoder


    // Measure performance of the second round
    printf("Second round\n");
    matrix_multiply((int*)0xf0000000, (int*)0xf0000400, (int*)0xf0000c00);

    for(int i = 0 ; i < MATRIX_SIZE * MATRIX_SIZE ; i++) {
        if(C[i] != D[i]) {
            printf("Error in %d\n", i);
            return 1;
        }
    }

    printf("Done\n");

    return 0;
}
