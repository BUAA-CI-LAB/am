#ifndef __TRAP_H__
#define __TRAP_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <la32r-csr.h>
#include "asm.h"
#include "regdef.h"
#include "cpu_cde.h"

__attribute__((noinline))
void nemu_assert(int cond) {
  if (!cond) _halt(1);
}

#endif
