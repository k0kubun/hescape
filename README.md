# Hescape

C library for fast HTML escape using SSE instruction, `pcmpestri`.

## Usage

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hescape.h"

int main(void) {
  uint8_t *dest, *src = "<>\"'&";
  size_t len = hesc_escape_html(&dest, src, strlen(src));
  printf("%s => %s\n", src, dest);
  if (len > strlen(src)) {
    free(dest);
  }
  return 0;
}

// <>"'& => &lt;&gt;&quot;&#39;&amp;
```

## License

MIT License
