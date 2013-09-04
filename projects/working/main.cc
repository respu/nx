//
// Copyright (C) 2013 Jacob McIntosh <nacitar at ubercpp dot com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/// @file main.cc
/// @brief Test project for things being worked on.

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "nx/application.h"
#include "nx/time.h"
#include "nx/constant.h"
#include "nx/bit_scan_reverse.h"
#include "nx/bit_scan_forward.h"
#include "nx/to_string.h"
#include "nx/population_count.h"
#include "nx/reverse.h"
#include "nx/digits.h"
#include "nx/literal.h"

template <unsigned int base, unsigned int power>
using IntPow = nx::Power<unsigned int, base, power>;

/// @brief The class for the test application
class MyApplication : public nx::Application {
 public:
  void timer_resolution_test() {
    nx::msec_t last = 0;
    while (true) {
      nx::msec_t now = nx::time::Fixed();
      if (last != now) {
        printf("%d", static_cast<unsigned int>(now));
        last = now;
      }
    }
  }

  void test_bitops(nx::uint_least64_t x) {
    std::cout << "BSF: " << nx::BitScanForward(x) << std::endl;
    std::cout << "BSR: " << nx::BitScanReverse(x) << std::endl;
    std::cout << "dig: " << nx::Digits(x) << std::endl;
  }

  int test_strings() {
    int sum = 0;
    std::string buf;
    for (int i = 0; i < 100000000; ++i) {
      sum += nx::ToString(-i, &buf);
      buf.clear();
    }
    return sum;
  }

  int main() {
    ArgumentVector& args = arguments();
    if (args.size() != 2) {
      std::cerr << "INVALID" << std::endl;
      return 1;
    }
    auto blit_val = 11110110_nx_b;
    auto pow_val = IntPow<10, 9>::value;
    nx::int_least64_t cmdline_val;
    {
      std::stringstream ss(args[1]);
      ss >> cmdline_val;
    }
    char buf[256];
    std::cout << "blit_val reverse: " << +nx::Reverse(blit_val) << std::endl;
    std::cout << "cmdline_val: " << cmdline_val << std::endl;
    std::cout << "power test: " << pow_val << std::endl;
    std::cout << "digits_test: " << nx::Digits(cmdline_val) << std::endl;
    std::cout << "tos test: " << nx::ToString(cmdline_val) << std::endl;
    std::cout << "tos pad: " << nx::ToString(cmdline_val, 10) << std::endl;
    int len = nx::ToString(-1234, buf);
    buf[len] = '\0';
    std::cout << "buf: " << buf << std::endl;

    return 0;
  }
};

/// @brief Function to lazy-load the application; required by nx library.
nx::Application& nx::GetApplication() {
  static MyApplication app;
  return app;
}
