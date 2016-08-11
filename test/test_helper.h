#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../hescape.h"

static int assertions, failures;

void
teardown()
{
  printf("\n\n%d assertions, %d failures\n\n", assertions, failures);
  exit(failures);
}

void
expect_str(const char *expected, const char *actual, const char *file, int line)
{
  assertions++;
  if (!strcmp(expected, actual)) {
    printf(".");
  } else {
    failures++;
    fflush(stdout);
    fprintf(stderr, "F\n\n%s:%d:\n", file, line);
    fprintf(stderr, "  Expected '%s' but got '%s'.", expected, actual);
    teardown();
  }
}

void
expect_int(int expected, int actual, const char *file, int line)
{
  assertions++;
  if (expected == actual) {
    printf(".");
  } else {
    failures++;
    fflush(stdout);
    fprintf(stderr, "F\n\n%s:%d:\n", file, line);
    fprintf(stderr, "  Expected '%d' but got '%d'.", expected, actual);
    teardown();
  }
}

#define TEST_RETURN(expected, src) { \
  hesc_buf buf = HESC_BUF_INIT; \
  int actual = hesc_escape_html(&buf, src, strlen(src)); \
  expect_int(expected, actual, __FILE__, __LINE__); \
}

#define TEST_RESULT(expected, src) { \
  hesc_buf buf = HESC_BUF_INIT; \
  hesc_escape_html(&buf, src, strlen(src)); \
  expect_str(expected, buf.str, __FILE__, __LINE__); \
  expect_int(strlen(buf.str), buf.len, __FILE__, __LINE__); \
}

void
setup()
{
  assertions = 0;
  failures   = 0;
  printf("\n# Running:\n\n");
}
