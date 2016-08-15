#include "test_helper.h"

void
test_escaped_string()
{
  TEST_RESULT("", "");
  TEST_RESULT("hello world", "hello world");
  TEST_RESULT("&quot;&#39;&amp;&lt;&gt;", "\"'&<>");
  TEST_RESULT("&lt;script&gt;&lt;/script&gt;", "<script></script>");
  TEST_RESULT("&lt;p&gt;こんにちは&lt;/p&gt;", "<p>こんにちは</p>");
  TEST_RESULT("&lt;123456789012345", "<123456789012345");
  TEST_RESULT("&lt;1234567890123456", "<1234567890123456");
  TEST_RESULT("012345678901234&#39;", "012345678901234'");
  TEST_RESULT("012345678901234&#39;6", "012345678901234'6");
  TEST_RESULT("012345678901234567890123456789&amp;&amp;", "012345678901234567890123456789&&");
}

int
main(void)
{
  setup();
  test_escaped_string();
  teardown();
  return 0;
}
