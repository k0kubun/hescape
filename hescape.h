#ifndef HESCAPE_H
#define HESCAPE_H

#include <sys/types.h>
#include <stdint.h>

extern char __hesc_initbuf[];
#define HESC_BUF_INIT { __hesc_initbuf, 0 }

typedef struct {
  char *str;
  size_t len;
} hesc_buf;

extern int hesc_escape_html(hesc_buf *dest, const uint8_t *src, size_t len);

#endif
