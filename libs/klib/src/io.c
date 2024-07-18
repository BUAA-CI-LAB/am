#include <klib.h>
#include <driver/confreg.h>

uint32_t uptime() {
  return read_time();
}
