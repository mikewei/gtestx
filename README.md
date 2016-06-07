## GTESTX - A C++ benchmark extension for gtest

Benchmark or performance-testing is often as important as unit test in many environment such as server side development. GTESTX is just a convenient and simple C++ benchmark tool. It is based on gtest framework, so if you are familiar with gtest you can write benchmark code easily only by knowing some extended macros.

### Examples

PERF\_TEST (similar with TEST macro) is a simplest gtestx macro. By default the function body will be called in a dead loop for a period of time and then the performance counter recorded is reported.

This is an example:

```C++
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
```

We can run it as normal gtest tests:

```
$ ./gtestx_examples --gtest_filter='SimpleTest*'
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from SimpleTest
[ RUN      ] SimpleTest.SysCall
[       OK ] SimpleTest.SysCall (0 ms)
[ RUN      ] SimpleTest.SysCallPerf
      count: 25,369,636
       time: 1.508939 s
         HZ: 16,812,897.009090
       1/HZ: 0.000000059 s
[       OK ] SimpleTest.SysCallPerf (1509 ms)
[----------] 2 tests from SimpleTest (1509 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (1510 ms total)
[  PASSED  ] 2 tests.
```

Another example:

```C++
class StdMapTest : public testing::Test
{
protected:
  virtual ~StdMapTest() {}
  virtual void SetUp() override {
    for (int i = 0; i < 1000; i++) {
      map_.emplace(i, 1);
    }
  }
  virtual void TearDown() override {}

  std::map<int,int> map_;
};

PERF_TEST_F(StdMapTest, FindPerf)
{
  map_.find(999);
}
```

### Macros

Almost all gtest macro has a GTESTX version:

- PERF\_TEST
- PERF\_TEST\_F
- PERF\_TEST\_P
- TYPED\_PERF\_TEST
- TYPED\_PERF\_TEST\_P

And there are some more macros which can be used with additional options:

- PERF\_TEST\_OPT
- PERF\_TEST\_F\_OPT
- PERF\_TEST\_P\_OPT
- TYPED\_PERF\_TEST\_OPT
- TYPED\_PERF\_TEST\_P\_OPT

You can learn how to use them by read example code in _examples_ directory.

### Dependencies

- gtest  (>= 1.7 is well tested)
- gflags (>= 2.0 is well tested)

### Compile

Actually all gtestx code (without tests and examples) are only two files - gtestx.h and gtestx.cc, so you can import them into your projects easily.

#### Compile using Bazel

A [Bazel](http://bazel.io) BUILD file is also available, you can use it to build tests and examples or linked it to other bazel project. Note that this bazel BUILD depends on gtest and gflags modules which are modified-for-bazel version on my github.

For example you can build examples as follows:

```
mkdir work
cd work
touch WORKSPACE
git clone https://github.com/mikewei/gtestx.git
git clone https://github.com/mikewei/third_party.git
bazel build gtestx:examples
```

#### Compile using make

You can also build tests and examples using Makefile:

```
git clone https://github.com/mikewei/gtestx.git
cd examples
# change some directory setting in Makefile
make
```

### Run

Run GTESTX benchmark tests just as normal gtest tests. e.g.

	./gtestx_examples --gtest_filters='SimpleTest.*'

Or run within bazel:

	bazel run gtestx:examples -- --gtest_filter='SimpleTest.*'

Also GTESTX supply two more command line options:

	-hz CALLS_PER_SECOND
	-time RUNNING_MILLISECONDS

For example if you want run SimpleTest with 10000 times/sec for 10 seconds, use commonds bellow:

	./gtestx_examples --gtest_filters='SimpleTest.*' -hz=10000 -time=10000


### Tutorial (Chinese)

这里有一篇介绍[如何使用gtestx的文章](http://codinginet.com/articles/view/201606-use_gtestx_for_benchmark)。
