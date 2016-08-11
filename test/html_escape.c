#include "test_helper.h"

void
test_return_value()
{
  TEST_RETURN(0, "hello world");
  TEST_RETURN(1, "<");
  TEST_RETURN(1, ">");
  TEST_RETURN(1, "&");
  TEST_RETURN(1, "\"");
  TEST_RETURN(1, "'");
  TEST_RETURN(1, "123456789012345>");
  TEST_RETURN(0, "/");
  TEST_RETURN(1, "<script></script>");
}

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
  test_return_value();
  teardown();
  return 0;
}
