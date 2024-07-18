#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) && !defined(__PLATFORM_NATIVE__)

static unsigned long int next = 1;

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

unsigned long time()
{
    panic("not imple\n");
}

void srand(unsigned int seed) {
  next = seed;
}

int rand(void) {
  next = next * 1103515245 + 12345;
  return next;//(unsigned int)(next/65536) % 32768;
}

void *malloc(size_t size) {
static uintptr_t addr = 0;
  static bool flag = false;
  if (flag == false) {
    flag = true;
    addr = (uintptr_t)_heap.start;
  }

  size = (size_t)ROUNDUP(size, 8);
  uintptr_t old = addr;
  addr += size;
  assert((uintptr_t)_heap.start <= (uintptr_t)addr && (uintptr_t)addr < (uintptr_t)_heap.end);
  for (uint64_t *p = (uint64_t *)old; p != (uint64_t *)addr; p++) {
    *p = 0;
  }

  return (void*)old;
}

void free(void *ptr) {
}

#endif