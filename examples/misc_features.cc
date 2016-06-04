#include <sys/types.h>
#include <unistd.h>
#include "gtestx/gtestx.h"

// use XXX_OPT to override default HZ, TIME parameters
// e.g. test below will be called 10000 times/s for 5000 ms
PERF_TEST_OPT(MiscFeaturesTest, SysCallPerf, 10000, 5000)
{
  getppid();
}

// If you want to use ASSERT_XXX in PERF_XXX, add <<PERF_ABORT
// to ensure exit correctly when assertion failed
PERF_TEST(MiscFeaturesTest, PerfAbort)
{
  ASSERT_NE(0, getppid()) << PERF_ABORT;
}
