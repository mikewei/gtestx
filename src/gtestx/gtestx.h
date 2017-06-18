/* Copyright (c) 2012-2017, Bin Wei <bin@vip.qq.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * The names of its contributors may not be used to endorse or 
 * promote products derived from this software without specific prior 
 * written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef GTESTX_GTESTX_H_
#define GTESTX_GTESTX_H_

#include "gtest/gtest.h"
#include "gflags/gflags.h"

/* GTESTX is a benchmark extension for gtest
 * 
 * Added Macro:
 * - PERF_TEST
 * - PERF_TEST_F
 * - PERF_TEST_P
 * - TYPED_PERF_TEST
 * - TYPED_PERF_TEST_P
 * - {ALL_ABOVE}_OPT
 */

// conform to Semantic Versioning (http://semver.org)
#define GTESTX_MAJOR_VERSION 1
#define GTESTX_MINOR_VERSION 0
#define GTESTX_PATCH_VERSION 0
#define GTESTX_PRERLEASE_VERSION beta2

#define PERF_ABORT (Perf::Abort(), "Perf loop aborted!")

///////////////////////////////////////////////

#define _TO_STR(sym) #sym
#define _PERF_TEST_BODY(opt_hz, opt_time) \
    google::FlagSaver flag_saver;\
    if (opt_hz >= 0)\
      google::SetCommandLineOptionWithMode("hz", _TO_STR(opt_hz), google::SET_FLAG_IF_DEFAULT);\
    if (opt_time >= 0)\
      google::SetCommandLineOptionWithMode("time", _TO_STR(opt_time), google::SET_FLAG_IF_DEFAULT);\
    gtestx::Perf::Run();

#define PERF_TEST_CLASS_NAME_(test_case_name, test_name) \
  test_case_name##_##test_name##_PerfTest

///////////////////////////////////////////////

#define PERF_TEST_(test_case_name, test_name, parent_class, parent_id, opt_hz, opt_time) \
  class PERF_TEST_CLASS_NAME_(test_case_name, test_name) :\
    public parent_class, public gtestx::Perf {\
   public:\
    PERF_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
   private:\
    virtual void TestCode();\
  };\
  GTEST_TEST_(test_case_name, test_name, \
    PERF_TEST_CLASS_NAME_(test_case_name, test_name), parent_id) {\
    _PERF_TEST_BODY(opt_hz, opt_time)\
  }\
  void PERF_TEST_CLASS_NAME_(test_case_name, test_name)::TestCode()

#define PERF_TEST(test_case_name, test_name) \
  PERF_TEST_(test_case_name, test_name, \
    ::testing::Test, ::testing::internal::GetTestTypeId(), \
    -1, -1)

#define PERF_TEST_OPT(test_case_name, test_name, opt_hz, opt_time) \
  PERF_TEST_(test_case_name, test_name, \
    ::testing::Test, ::testing::internal::GetTestTypeId(), \
    opt_hz, opt_time)

#define PERF_TEST_F(test_fixture, test_name) \
  PERF_TEST_(test_fixture, test_name, \
    test_fixture, ::testing::internal::GetTypeId<test_fixture>(), \
    -1, -1)

#define PERF_TEST_F_OPT(test_fixture, test_name, opt_hz, opt_time) \
  PERF_TEST_(test_fixture, test_name, \
    test_fixture, ::testing::internal::GetTypeId<test_fixture>(), \
    opt_hz, opt_time)

///////////////////////////////////////////////

# define TEST_P_(test_case_name, test_name, parent_class) \
  class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) \
      : public parent_class { \
   public: \
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {} \
    virtual void TestBody(); \
   private: \
    static int AddToRegistry() { \
      ::testing::UnitTest::GetInstance()->parameterized_test_registry(). \
          GetTestCasePatternHolder<test_case_name>(\
              #test_case_name, __FILE__, __LINE__)->AddTestPattern(\
                  #test_case_name, \
                  #test_name, \
                  new ::testing::internal::TestMetaFactory< \
                      GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>()); \
      return 0; \
    } \
    static int gtest_registering_dummy_; \
    GTEST_DISALLOW_COPY_AND_ASSIGN_(\
        GTEST_TEST_CLASS_NAME_(test_case_name, test_name)); \
  }; \
  int GTEST_TEST_CLASS_NAME_(test_case_name, \
                             test_name)::gtest_registering_dummy_ = \
      GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::AddToRegistry(); \
  void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()

# define PERF_TEST_P_(test_case_name, test_name, opt_hz, opt_time) \
  class PERF_TEST_CLASS_NAME_(test_case_name, test_name) :\
    public test_case_name, public gtestx::Perf {\
   public:\
    PERF_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
   private:\
    virtual void TestCode();\
  };\
  TEST_P_(test_case_name, test_name, \
    PERF_TEST_CLASS_NAME_(test_case_name, test_name)) {\
    _PERF_TEST_BODY(opt_hz, opt_time)\
  }\
  void PERF_TEST_CLASS_NAME_(test_case_name, test_name)::TestCode()

#define PERF_TEST_P(test_case_name, test_name) \
  PERF_TEST_P_(test_case_name, test_name, -1, -1)

#define PERF_TEST_P_OPT(test_case_name, test_name, opt_hz, opt_time) \
  PERF_TEST_P_(test_case_name, test_name, opt_hz, opt_time)

///////////////////////////////////////////////

#define TYPED_TEST_(CaseName, TestName, ParentClass, FixtureClass) \
  template <typename gtest_TypeParam_> \
  class GTEST_TEST_CLASS_NAME_(CaseName, TestName) \
     : public ParentClass<gtest_TypeParam_> { \
   private: \
    typedef ParentClass<gtest_TypeParam_> TestFixture; \
    typedef gtest_TypeParam_ TypeParam; \
    virtual void TestBody(); \
  }; \
  bool gtest_##CaseName##_##TestName##_registered_ = \
    ::testing::internal::TypeParameterizedTest< \
      FixtureClass, \
      ::testing::internal::TemplateSel< \
        GTEST_TEST_CLASS_NAME_(CaseName, TestName)>, \
      GTEST_TYPE_PARAMS_(CaseName)>::Register(\
        "", #CaseName, #TestName, 0); \
  template <typename gtest_TypeParam_> \
  void GTEST_TEST_CLASS_NAME_(CaseName, TestName)<gtest_TypeParam_>::TestBody()

#define TYPED_PERF_TEST_(test_case_name, test_name, opt_hz, opt_time) \
  template <typename T>\
  class PERF_TEST_CLASS_NAME_(test_case_name, test_name) :\
    public test_case_name<T>, public gtestx::Perf {\
   public:\
    PERF_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
   private:\
    typedef test_case_name<T> TestFixture; \
    typedef T TypeParam; \
    virtual void TestCode();\
  };\
  TYPED_TEST_(test_case_name, test_name, \
    PERF_TEST_CLASS_NAME_(test_case_name, test_name), test_case_name) {\
    _PERF_TEST_BODY(opt_hz, opt_time)\
  }\
  template <typename T>\
  void PERF_TEST_CLASS_NAME_(test_case_name, test_name)<T>::TestCode()

#define TYPED_PERF_TEST_CASE(CaseName, Types) TYPED_TEST_CASE(CaseName, Types)

#define TYPED_PERF_TEST(test_case_name, test_name) \
  TYPED_PERF_TEST_(test_case_name, test_name, -1, -1)

#define TYPED_PERF_TEST_OPT(test_case_name, test_name, opt_hz, opt_time) \
  TYPED_PERF_TEST_(test_case_name, test_name, opt_hz, opt_time)

///////////////////////////////////////////////

#define TYPED_TEST_P_(CaseName, TestName, ParentClass) \
  namespace GTEST_CASE_NAMESPACE_(CaseName) { \
    template <typename gtest_TypeParam_> \
    class TestName : public ParentClass<gtest_TypeParam_> { \
     private: \
      typedef ParentClass<gtest_TypeParam_> TestFixture; \
      typedef gtest_TypeParam_ TypeParam; \
      virtual void TestBody(); \
    }; \
    static bool gtest_##TestName##_defined_ = \
    GTEST_TYPED_TEST_CASE_P_STATE_(CaseName).AddTestName(\
        __FILE__, __LINE__, #CaseName, #TestName); \
  } /* NOLINT */ \
  template <typename gtest_TypeParam_> \
  void GTEST_CASE_NAMESPACE_(CaseName)::TestName<gtest_TypeParam_>::TestBody()

#define TYPED_PERF_TEST_P_(test_case_name, test_name, opt_hz, opt_time) \
  template <typename T>\
  class PERF_TEST_CLASS_NAME_(test_case_name, test_name) :\
    public test_case_name<T>, public gtestx::Perf {\
   public:\
    PERF_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
   private:\
    typedef test_case_name<T> TestFixture; \
    typedef T TypeParam; \
    virtual void TestCode();\
  };\
  TYPED_TEST_P_(test_case_name, test_name, \
    PERF_TEST_CLASS_NAME_(test_case_name, test_name)) {\
    _PERF_TEST_BODY(opt_hz, opt_time)\
  }\
  template <typename T>\
  void PERF_TEST_CLASS_NAME_(test_case_name, test_name)<T>::TestCode()

#define TYPED_PERF_TEST_CASE_P(ClassName) TYPED_TEST_CASE_P(CaseName)

#define TYPED_PERF_TEST_P(test_case_name, test_name) \
  TYPED_PERF_TEST_P_(test_case_name, test_name, -1, -1)

#define TYPED_PERF_TEST_P_OPT(test_case_name, test_name, opt_hz, opt_time) \
  TYPED_PERF_TEST_P_(test_case_name, test_name, opt_hz, opt_time)

#define REGISTER_TYPED_PERF_TEST_CASE_P(CaseName, ...) \
  REGISTER_TYPED_TEST_CASE_P(CaseName, __VA_ARGS__)

#define INSTANTIATE_TYPED_PERF_TEST_CASE_P(Prefix, CaseName, Types) \
  INSTANTIATE_TYPED_TEST_CASE_P(Prefix, CaseName, Types)

///////////////////////////////////////////////

#define PERF_TEST_MAIN \
  int main(int argc, char** argv) { \
    testing::InitGoogleTest(&argc, argv); \
    google::ParseCommandLineFlags(&argc, &argv, true); \
    return RUN_ALL_TESTS(); \
  }

///////////////////////////////////////////////

namespace gtestx {

class Perf {
 public:
  virtual ~Perf() {}
  virtual void TestCode() = 0;
  void Run();
  void Abort();

 private:
  size_t hz_;
  time_t time_;
  volatile bool stop_;
};

}  // namespace gtestx

#endif  // GTESTX_GTESTX_H_
