#include <stdio.h>
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

// Non-null and zero terminated buffer.
char __hesc_initbuf[1];

static const char *ESCAPED_STRING[] = {
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

static const char*
find_escaped_fast(const char *buf, const char *buf_end, int *found)
{
  static const char ranges[] = "\"&'<>";

  if (likely(buf_end - buf >= 16)) {
    __m128i ranges16 = _mm_loadu_si128((const __m128i *)ranges);
    size_t left = (buf_end - buf) & ~15;
    do {
      __m128i b16 = _mm_loadu_si128((void *)buf);
      int index = _mm_cmpestri(ranges16, 5, b16, 16, _SIDD_CMP_EQUAL_ANY);
      if (unlikely(index != 16)) {
        buf += index;
        *found = 1;
        break;
      }
      buf += 16;
      left -= 16;
    } while(likely(left != 0));
  }
  return buf;
}

/*
 * Replace characters according to the following rules.
 * This function can handle only ASCII-compatible string.
 *
 * " => &quot;
 * & => &amp;
 * ' => &#39;
 * < => &lt;
 * > => &gt;
 *
 * @return 1 if there is escaped characters, which means that dest->str is newly allocated.
 */
int
hesc_escape_html(hesc_buf *dest, const uint8_t *buf, size_t len)
{
  const uint8_t *head = buf, *buf_end = buf + len;
  int ret = 0, found;

  while (buf < buf_end) {
    buf = find_escaped_fast(buf, buf_end, &found);
    if (found) {
      if (HTML_ESCAPE_TABLE[*buf]) {
        ret = 1;
      }
      buf++;
    } else {
      break;
    }
  }

  while (buf < buf_end) {
    while (buf < buf_end && HTML_ESCAPE_TABLE[*buf] == 0)
      buf++;
    if (HTML_ESCAPE_TABLE[*buf]) {
      ret = 1;
    }
    buf++;
  }

  dest->str = (char *)head;
  dest->len = len;
  return ret;
}
