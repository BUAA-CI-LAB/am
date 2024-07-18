#include <am.h>
#include BOARD_H
#include <klib.h>
#include <driver/confreg.h>

inline uint64_t read_time() {
  return ind(CONFREG_TIMER_PADDR);
}
