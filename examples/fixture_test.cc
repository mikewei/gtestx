#include <sys/types.h>
#include <unistd.h>
#include <map>
#include "gtestx/gtestx.h"

class StdMapTest : public testing::Test
{
protected:
  virtual void SetUp() override {
    for (int i = 0; i < 1000; i++) {
      map_.emplace(i, 1);
    }
  }
  virtual void TearDown() override {
  }
  std::map<int,int> map_;
};

TEST_F(StdMapTest, Find)
{
  ASSERT_NE(map_.find(999), map_.end());
}

// use PERF_TEST_F for performance testing
PERF_TEST_F(StdMapTest, FindPerf)
{
  map_.find(999);
}
