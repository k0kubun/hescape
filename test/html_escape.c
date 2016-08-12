#include "test_helper.h"

void
test_escaped_string()
{
  TEST_RESULT("", "");
  TEST_RESULT("hello world", "hello world");
}

int
main(void)
{
  setup();
  test_escaped_string();
  teardown();
  return 0;
}
