// Copyright (c) [2021-2023] [Horizon Robotics][Horizon Bole].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     http: //www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#ifndef COMMUNICATION_SCOPE_GUARD_H_
#define COMMUNICATION_SCOPE_GUARD_H_

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

#include <functional>

namespace hobot {
namespace communication {

class DLLEXPORT ScopeGuard final {
 public:
  using GuardFunc = std::function<void(void)>;
  explicit ScopeGuard(GuardFunc func);
  ~ScopeGuard();

  ScopeGuard(const ScopeGuard &) = delete;
  ScopeGuard &operator=(const ScopeGuard &) = delete;

 private:
  GuardFunc at_exit_func_;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SCOPE_GUARD_H_
