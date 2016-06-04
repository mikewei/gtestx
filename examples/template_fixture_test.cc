#include <queue>
#include "gtestx/gtestx.h"

template <class T>
class QueueTest : public testing::Test
{
protected:
  virtual ~QueueTest() {}
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  std::queue<T> queue_;
};


typedef testing::Types<int, std::vector<char>> TestTypes;
TYPED_TEST_CASE(QueueTest, TestTypes);

TYPED_TEST(QueueTest, PushPop)
{
  this->queue_.push(TypeParam());
  this->queue_.pop();
}

// use TYPED_PERF_TEST for performance testing
TYPED_PERF_TEST(QueueTest, PushPopPerf)
{
  this->queue_.push(TypeParam());
  this->queue_.pop();
}
