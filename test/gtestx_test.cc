/* Copyright (c) 2012-2015, Bin Wei <bin@vip.qq.com>
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
#include "gtestx/gtestx.h"

TEST(Nop, test) {
}

PERF_TEST(Nop, perf) {
}

///////////////////////////////////////////////

class Empty : public testing::Test {
};

TEST_F(Empty, test) {
}

PERF_TEST_F(Empty, perf) {
}

///////////////////////////////////////////////

class EmptyP : public testing::TestWithParam<int> {
};

TEST_P(EmptyP, test) {
}

PERF_TEST_P(EmptyP, perf) {
}

INSTANTIATE_TEST_CASE_P(Inst, EmptyP, testing::Values(1, 2, 3));

///////////////////////////////////////////////

template <typename T>
class EmptyTmplt : public testing::Test {
};

typedef testing::Types<int, char, void*> TestTypes;
TYPED_TEST_CASE(EmptyTmplt, TestTypes);

TYPED_TEST(EmptyTmplt, Test) {
}

TYPED_PERF_TEST(EmptyTmplt, Perf) {
}

///////////////////////////////////////////////

template <typename T>
class EmptyTmpltP : public testing::Test {
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

