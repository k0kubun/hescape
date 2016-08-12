#include <stdio.h>
#include <string.h>
#include "../hescape.h"

int
main(void)
{
  const char *src = "1234567890<234567890";
  uint8_t *buf = NULL;
  hesc_escape_html(&buf, src, strlen(src));
  printf("result: '%s'\n", buf);
  return 0;
}
