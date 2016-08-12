#include <stdio.h>
#include <string.h>
#include "hescape.h"

#ifdef __SSE4_2__
# ifdef _MSC_VER
#  include <nmmintrin.h>
# else
#  include <x86intrin.h>
# endif
#endif

#if __GNUC__ >= 3
# define likely(x) __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#else
# define likely(x) (x)
# define unlikely(x) (x)
#endif

static const uint8_t *ESCAPED_STRING[] = {
  "",
  "&quot;",
  "&amp;",
  "&#39;",
  "&lt;",
  "&gt;",
};

/*
 * Given ASCII-compatible character, return index of ESCAPED_STRING.
 *
 * " (34) => 1 (&quot;)
 * & (38) => 2 (&amp;)
 * ' (39) => 3 (&#39;)
 * < (60) => 4 (&lt;)
 * > (62) => 5 (&gt;)
 */
static const char HTML_ESCAPE_TABLE[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 5, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

size_t
hesc_escape_html(uint8_t **dest, const uint8_t *buf, size_t size)
{
  const uint8_t *esc;
  size_t rsize = 0, esize = 1, esc_i;
  uint8_t *rbuf = (uint8_t *)malloc(sizeof(uint8_t) * (size + esize));

  for (size_t i = 0; i < size; i++) {
    if ((esc_i = HTML_ESCAPE_TABLE[buf[i]])) {
      esc = ESCAPED_STRING[esc_i];
      esize += strlen(esc);
      rbuf = (uint8_t *)realloc(rbuf, sizeof(uint8_t) * (size + esize));
      memmove(rbuf + rsize, esc, strlen(esc));
      rsize += strlen(esc);
    } else {
      rbuf[rsize] = buf[i];
      rsize++;
    }
  }
  rbuf[rsize] = '\0';

  *dest = rbuf;
  return rsize;
}
