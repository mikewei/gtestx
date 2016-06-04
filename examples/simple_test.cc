#include <sys/types.h>
#include <unistd.h>
#include "gtestx/gtestx.h"

TEST(SimpleTest, SysCall)
{
  ASSERT_NE(0, getppid());
}

// Just like TEST macro, use PERF_TEST for performance testing
PERF_TEST(SimpleTest, SysCallPerf)
{
  getppid();
}
