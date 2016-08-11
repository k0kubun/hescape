#include "hescape.h"

// Non-null and zero terminated buffer.
char __hesc_initbuf[1];

/*
 * Replace characters according to the following rules.
 *
 * ' => &#39;
 * & => &amp;
 * " => &quot;
 * < => &lt;
 * > => &gt;
 *
 * @return 1 if there are no escaped characters.
 */
int
hesc_escape_html(hesc_buf *dest, const uint8_t *src, size_t len)
{
  dest->str = (char *)src;
  dest->len = len;
  return 0;
}
