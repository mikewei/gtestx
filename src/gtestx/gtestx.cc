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
 *     * The name of of its contributors may not be used to endorse or 
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
#include <unistd.h>
#include <sys/time.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <thread>  // NOLINT
#include "gtestx/gtestx.h"

#define TV2US(ptv) ((ptv)->tv_sec * 1000000 + (ptv)->tv_usec)

DEFINE_uint64(hz, 0, "loops per second");
DEFINE_uint64(time, 1500, "testing time long (ms)");

namespace {

class TokenBucket {
 public:
  TokenBucket(uint32_t tokens_per_sec, uint32_t bucket_size, uint32_t init_tokens);
  void Gen(struct timeval *tv_now = NULL);
  bool Get(uint32_t need_tokens);

 private:
  uint32_t tokens_per_sec_;
  uint32_t bucket_size_;
  int64_t token_count_;
  uint64_t last_gen_time_;
  uint64_t last_calc_delta_;
};

TokenBucket::TokenBucket(uint32_t tokens_per_sec,
                         uint32_t bucket_size,
                         uint32_t init_tokens) {
  struct timeval tv;
  tokens_per_sec_ = tokens_per_sec;
  bucket_size_ = bucket_size;
  token_count_ = init_tokens;
  gettimeofday(&tv, NULL);
  last_gen_time_ = TV2US(&tv);
  last_calc_delta_ = 0;
}

void TokenBucket::Gen(struct timeval *tv_now) {
  struct timeval tv;
  uint64_t us_now, us_past;
  uint64_t new_tokens, calc_delta;
  int64_t new_token_count;

  if (tv_now == NULL) {
    tv_now = &tv;
    gettimeofday(tv_now, NULL);
  }
  us_now = TV2US(tv_now);
  if (us_now < last_gen_time_) {
    last_gen_time_ = us_now;
    return;
  }

  us_past = us_now - last_gen_time_;
  new_tokens = (((uint64_t)tokens_per_sec_ * us_past + last_calc_delta_) / 1000000);
  calc_delta = (((uint64_t)tokens_per_sec_ * us_past + last_calc_delta_) % 1000000);

  last_gen_time_ = us_now;
  last_calc_delta_ = calc_delta;
  new_token_count = token_count_ + new_tokens;
  if (new_token_count < token_count_) {
    token_count_ = bucket_size_;
    return;
  }
  if (new_token_count > bucket_size_) {
    new_token_count = bucket_size_;
  }
  token_count_ = new_token_count;

  return;
}

bool TokenBucket::Get(uint32_t need_tokens) {
  if (token_count_ < (int64_t)need_tokens) {
    return false;
  }
  token_count_ -= need_tokens;
  return true;
}

}  // anonymous namespace

namespace gtestx {

void Perf::Run() {
  struct timeval tv_begin, tv_end, tv_use;
  unsigned long long count = 0;  // NOLINT

  stop_ = false;
  hz_ = FLAGS_hz;
  time_ = FLAGS_time;

  TokenBucket tb(hz_, hz_/5, hz_/500);
  std::thread alarm([this] {
    int unit = 10;
    for (int ms = time_; !stop_ && ms > 0; ms -= unit)
      std::this_thread::sleep_for(std::chrono::milliseconds(ms > unit ? unit : ms));
    stop_ = true;
  });

  gettimeofday(&tv_begin, NULL);

  while (!stop_) {
    if (!hz_ || tb.Get(1)) {
      TestCode();
      count++;
    } else {
      usleep(1000);
      tb.Gen();
    }
  }

  gettimeofday(&tv_end, NULL);
  timersub(&tv_end, &tv_begin, &tv_use);

  setlocale(LC_ALL, "");
  printf("      count: %'llu\n", count);
  printf("       time: %'d.%06d s\n", static_cast<int>(tv_use.tv_sec),
                                      static_cast<int>(tv_use.tv_usec));
  printf("         HZ: %'f\n", count/(tv_use.tv_sec+0.000001*tv_use.tv_usec));
  printf("       1/HZ: %'.9f s\n", (tv_use.tv_sec+0.000001*tv_use.tv_usec)/count);

  alarm.join();
}

void Perf::Abort() {
  stop_ = true;
}

}  // namespace gtestx

PERF_TEST_MAIN

