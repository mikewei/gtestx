#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include "gtestx/gtestx.h"

class HashMapTest : public testing::TestWithParam<float>
{
protected:
  virtual ~HashMapTest() {}
  virtual void SetUp() override {
    map_.max_load_factor(GetParam());
    for (int i = 0; i < 1000; i++) {
      map_.emplace(i, 1);
    }
  }
  virtual void TearDown() override {}

  std::unordered_map<int,int> map_;
  int search_key_ = 0;
};

INSTANTIATE_TEST_CASE_P(LoadFactor, HashMapTest, 
                        testing::Values(3.0, 1.0, 0.3));

TEST_P(HashMapTest, Find)
{
  ASSERT_NE(map_.find(999), map_.end());
}

// use PERF_TEST_P for performance testing
PERF_TEST_P(HashMapTest, FindPerf)
{
  map_.find(++search_key_);
}
