# Hescape [![wercker status](https://app.wercker.com/status/cba5f4a964c895e34be39f184af59964/s/master "wercker status")](https://app.wercker.com/project/byKey/cba5f4a964c895e34be39f184af59964)

C library for fast HTML escape using SSE instruction, `pcmpestri`. For ruby, you can use this via [hescape gem](https://github.com/k0kubun/hescape-ruby).

## Benchmark

See [the result](https://app.wercker.com/k0kubun/hescape/runs/build/57b0919ce8960a01001b323a?step=57b091afc164c60001dd5d6f) of [this benchmark](https://github.com/k0kubun/hescape/blob/ee34bf9c70301d4fa43ef9d88b5dafddffebc865/benchmark/benchmark.c).

[Without escaped characters](https://github.com/k0kubun/hescape/blob/ee34bf9c70301d4fa43ef9d88b5dafddffebc865/benchmark/benchmark.c#L81), hescape is 7.76x faster than [houdini](https://github.com/vmg/houdini).

```
==============================================
 no escape (N=300000)
==============================================
hescape: 0.024481 s (12254293.3 i/s)
houdini: 0.189860 s (1580109.3 i/s)

hescape is 7.76x faster
```

[With escaped characters](https://github.com/k0kubun/hescape/blob/ee34bf9c70301d4fa43ef9d88b5dafddffebc865/benchmark/benchmark.c#L84), which is escaping [vmg/houdini/tools/wikipedia\_table.txt](https://github.com/vmg/houdini/blob/59727b85553b70d468743076219e620d6c0d3cad/tools/wikipedia_table.txt), hescape is 1.62x faster than [houdini](https://github.com/vmg/houdini).

```
==============================================
 wikipedia table (N=10000)
==============================================
hescape: 0.232239 s (43059.0 i/s)
houdini: 0.375880 s (26604.2 i/s)

hescape is 1.62x faster
```

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

## Note

Many ideas except `pcmpestri` are originally from [vmg/houdini](https://github.com/vmg/houdini).

## License

MIT License
