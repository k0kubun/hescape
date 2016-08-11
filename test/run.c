#include <stdio.h>
#include <string.h>
#include "../hescape.h"

int
main(void)
{
  const char *src = "1234567890<234567890";
  hesc_buf buf = HESC_BUF_INIT;
  hesc_escape_html(&buf, src, strlen(src));
  printf("result: '%s'\n", buf.str);
  return 0;
}
