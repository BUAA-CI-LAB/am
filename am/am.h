/*
 * The Nexus Abstract Machine Architecture (AM)
 * A portable abstraction of a bare-metal computer
 */

#ifndef __AM_H__
#define __AM_H__

#include <stdint.h>
#include <stddef.h>
#include ARCH_H // "arch/x86-qemu.h", "arch/native.h", ...

#ifdef __cplusplus
extern "C" {
#endif

// ===================== Constants and Structs =======================
// typedef unsigned long uintptr_t;
// typedef long intptr_t;
// typedef unsigned long size_t;
// typedef unsigned int uint32_t;

enum {
  _EVENT_NULL = 0,
  _EVENT_ERROR,
  _EVENT_IRQ_TIMER,
  _EVENT_IRQ_IODEV,
  _EVENT_PAGEFAULT,
  _EVENT_SYSCALL,
  _EVENT_YIELD,
};

enum {
  _PROT_NONE  = 0x1, // no access
  _PROT_READ  = 0x2, // can read
  _PROT_WRITE = 0x4, // can write
  _PROT_EXEC  = 0x8, // can execute
};

// Memory area for [@start, @end)
typedef struct _Area {
  void *start, *end;
} _Area;

// An event of type @event, caused by @cause of pointer @ref
typedef struct _Event {
  int event;
  uintptr_t cause, ref;
  const char *msg;
} _Event;

// Arch-dependent processor context
typedef struct _Context _Context;

// A protected address space with user memory @area
// and arch-dependent @ptr
typedef struct _AddressSpace {
  size_t pgsize;
  _Area area;
  void *ptr;
} _AddressSpace;

// ====================== Turing Machine (TRM) =======================

extern _Area _heap;
char _getc();
void _putc(char ch);
void _halt(int code) __attribute__((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif
