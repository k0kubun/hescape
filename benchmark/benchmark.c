#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../hescape.h"
#include "houdini/houdini.h"

char*
strcont(const char *str, int count)
{
  char *ret = (char *)malloc(sizeof(char) * (strlen(str) * count + 1));
  ret[0] = '\0';
  for (int i = 0; i < count; i++) {
    strcat(ret, str);
  }
  return ret;
}

char*
readfile(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  rewind(fp);

  char *ret = malloc(sizeof(char) * (size + 1));
  if (fread(ret, sizeof(char), size, fp) == 0) {
    fprintf(stderr, "No characters couldn't be read from %s.\n", filename);
    exit(1);
  };
  return ret;
}

double
measure_time(const char *str, long iteration, void (* escape_func)(const char *))
{
  struct timespec start, end;
  double ret = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (long i = 0; i < iteration; i++) {
    escape_func(str);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  ret += end.tv_sec - start.tv_sec;
  ret += 1e-9 * (end.tv_nsec - start.tv_nsec);
  return ret;
}

void
escape_with_hescape(const char *str)
{
  uint8_t *buf = NULL;
  hesc_escape_html(&buf, str, strlen(str));
}

void
escape_with_houdini(const char *str)
{
  gh_buf buf = GH_BUF_INIT;
  houdini_escape_html0(&buf, str, strlen(str), 0);
}

void
bench_escape(const char *label, const char *str, long iteration)
{
  printf("==============================================\n %s (size=%ld)\n", label, strlen(str));
  printf("==============================================\n");

  double hesc_sec = measure_time(str, iteration, &escape_with_hescape);
  printf("hescape: %f s (%.1f i/s)\n", hesc_sec, iteration / hesc_sec);

  double houd_sec = measure_time(str, iteration, &escape_with_houdini);
  printf("houdini: %f s (%.1f i/s)\n\n", houd_sec, iteration / houd_sec);

  printf("hescape is %.2fx faster\n\n", houd_sec / hesc_sec);
}

int
main(void)
{
  // if size is <= 15, it does not use pcmpestri.
  bench_escape("no escape 15", "012345678912345", 3000000);
  bench_escape("no escape 16", "0123456789123456", 3000000);
  bench_escape("all escape 15", "<<<<<<<<<<<<<<<", 300000);
  bench_escape("all escape 16", "<<<<<<<<<<<<<<<<", 300000);

  // if escaped over twice, it allocates 1.5x memory
  bench_escape("1 escape", "<123456789123456", 300000);
  bench_escape("2 escape", "<<23456789123456", 300000);
  bench_escape("3 escape", "<<<3456789123456", 300000);

  // benchmark with reliable length
  bench_escape("no escape 1000", strcont("0123456789", 100), 300000);
  bench_escape("10% escape", strcont("'123456789", 100), 80000);
  bench_escape("all escape", strcont("><&\"'", 200), 20000);

  // houdini's benchmark
  bench_escape("wikipedia table", readfile("benchmark/wikipedia_table.txt"), 10000);

  return 0;
}
