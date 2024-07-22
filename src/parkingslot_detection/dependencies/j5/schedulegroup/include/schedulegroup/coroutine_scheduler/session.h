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

#ifndef SCHEDULEGROUP_COROUTINE_SCHEDULER_SESSION_H_
#define SCHEDULEGROUP_COROUTINE_SCHEDULER_SESSION_H_

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "schedulegroup/coroutine_scheduler/coroutine_scheduler.h"
#include "schedulegroup/schedule_group.h"
#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/task.h"

namespace hobot {
namespace schedulegroup {

int Open(std::string &pathname, int flags);
int Open(std::string &pathname, int flags, mode_t mode);
int Open(const char *pathname, int flags);
int Open(const char *pathname, int flags, mode_t mode);
int Socket(int domain, int type, int protocol);
int Listen(int fd, int backlog);
int Bind(int fd, const struct sockaddr *addr, socklen_t addrlen);
int Close(int fd);
int Accept(int fd, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int fd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Recv(int fd, void *buf, size_t len, int flags, int64_t timeout_us = -1);
ssize_t RecvFrom(int fd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen,
                 int64_t timeout_us = -1);
ssize_t Send(int fd, const void *buf, size_t len, int flags,
             int64_t timeout_us = -1);
ssize_t SendTo(int fd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen,
               int64_t timeout_us = -1);
ssize_t Read(int fd, void *buf, size_t count, int64_t timeout_us = -1);
ssize_t Write(int fd, const void *buf, size_t count, int64_t timeout_us = -1);

}  // namespace schedulegroup
}  // namespace hobot
#endif  // SCHEDULEGROUP_COROUTINE_SCHEDULER_SESSION_H_
