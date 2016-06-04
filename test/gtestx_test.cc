#include "gtestx/gtestx.h"

TEST(Nop, test) {
}

PERF_TEST(Nop, perf) {
}

///////////////////////////////////////////////

class Empty : public testing::Test
{
};

TEST_F(Empty, test) {
}

PERF_TEST_F(Empty, perf) {
}

///////////////////////////////////////////////

class EmptyP : public testing::TestWithParam<int>
{
};

TEST_P(EmptyP, test) {
}

PERF_TEST_P(EmptyP, perf) {
}

INSTANTIATE_TEST_CASE_P(Inst, EmptyP, testing::Values(1, 2, 3));

///////////////////////////////////////////////

template <typename T>
class EmptyTmplt : public testing::Test
{
};

typedef testing::Types<int, char, void*> TestTypes;
TYPED_TEST_CASE(EmptyTmplt, TestTypes);

TYPED_TEST(EmptyTmplt, Test) {
}

TYPED_PERF_TEST(EmptyTmplt, Perf) {
}

///////////////////////////////////////////////

template <typename T>
class EmptyTmpltP : public testing::Test
{
};

TYPED_TEST_CASE_P(EmptyTmpltP);

TYPED_TEST_P(EmptyTmpltP, Test) {
}

TYPED_PERF_TEST_P(EmptyTmpltP, Perf) {
}

REGISTER_TYPED_TEST_CASE_P(EmptyTmpltP, Test, Perf);

typedef testing::Types<int, char, void*> TestTypesP;
INSTANTIATE_TYPED_TEST_CASE_P(TestEmptyTmpltP, EmptyTmpltP, TestTypesP);

///////////////////////////////////////////////

//PERF_TEST_MAIN
