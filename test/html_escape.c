#include "test_helper.h"

void
test_html_escape()
{
  TEST_ESCAPE("", "");
  TEST_ESCAPE("hello world", "hello world");
}

int
main(void)
{
  setup();
  test_html_escape();
  teardown();
  return 0;
}
