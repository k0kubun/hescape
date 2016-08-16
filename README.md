# Hescape [![Build Status](https://travis-ci.org/k0kubun/hescape.svg?branch=master)](https://travis-ci.org/k0kubun/hescape)

C library for fast HTML escape using SSE instruction, `pcmpestri`. For ruby, you can use this via [hescape gem](https://github.com/k0kubun/hescape-ruby).

## API

Hescape provides only one API, `hesc_escape_html`. It may change at any time since this project is experimental for now.

```c
size_t hesc_escape_html(uint8_t **dest, const uint8_t *src, size_t size);
```

Given `src` and `size`, it stores a pointer for the escaped result to `dest` and returns its size.
Note that `hesc_escape_html` allocates new memory only when `src` has characters to be escaped.
So you need to free `dest` when return value is larger than `size`.

It escapes `src` with the following rules.

```
  " --> &quot;
  & --> &amp;
  ' --> &#39;
  < --> &lt;
  > --> &gt;
```

It's designed to be the same as `CGI.escapeHTML` in Ruby.

## Benchmark

See [the result](https://travis-ci.org/k0kubun/hescape/jobs/152685822) of [this benchmark](https://github.com/k0kubun/hescape/blob/62d990360d7b503f43c6bef65a4fb73bdf81a483/benchmark/benchmark.c).

```
==============================================
 no escape (N=300000)
==============================================
hescape: 0.065899 s (4552446.6 i/s)
houdini: 0.169504 s (1769864.6 i/s)

hescape is 2.57x faster

==============================================
 10% escape (N=80000)
==============================================
hescape: 0.184101 s (434543.7 i/s)
houdini: 0.309255 s (258686.3 i/s)

hescape is 1.68x faster

==============================================
 all escape (N=20000)
==============================================
hescape: 0.297408 s (67247.8 i/s)
houdini: 0.296916 s (67359.0 i/s)

hescape is 1.00x faster

==============================================
 wikipedia table (N=10000)
==============================================
hescape: 0.313769 s (31870.6 i/s)
houdini: 0.468118 s (21362.1 i/s)

hescape is 1.49x faster
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
