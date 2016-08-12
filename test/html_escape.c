#include "test_helper.h"

void
test_escaped_string()
{
  TEST_RESULT("", "");
  TEST_RESULT("hello world", "hello world");
  TEST_RESULT("&quot;&#39;&amp;&lt;&gt;", "\"'&<>");
  TEST_RESULT("&lt;script&gt;&lt;/script&gt;", "<script></script>");
}

int
main(void)
{
  setup();
  test_escaped_string();
  teardown();
  return 0;
}
