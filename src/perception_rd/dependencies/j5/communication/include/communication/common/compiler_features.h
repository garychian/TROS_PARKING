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

// This is for controling compiler features
#ifndef COMMUNICATION_COMMON_COMPILER_FEATURES_H_
#define COMMUNICATION_COMMON_COMPILER_FEATURES_H_

// To control whether warning whose id is `w` is emitted or not on windows.
// For example, if you want to suppress one waning which comes from some header
// files, please consider following:
//
// HLOG_MSVC_SUPPRESS_WARNING_WITH_PUSH(id)
// #include <3rd.h>
// HLOG_MSVC_SUPPRESS_WARNING_POP
//
// If you want suppres multiple wanings one at the same time, please consifer
// following:
//
// HLOG_MSVC_SUPPRESS_WARNING_WITH_PUSH(id1)
// HLOG_MSVC_SUPPRESS_WARNING_WITH_PUSH(id2)
// #include <3rd.h>
// HLOG_MSVC_SUPPRESS_WARNING_POP
// HLOG_MSVC_SUPPRESS_WARNING_POP

#if defined _WIN32 && defined _MSC_VER
# define COMM_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
# define COMM_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
# define COMM_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
# define COMM_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) \
          COMM_MSVC_SUPPRESS_WARNING_PUSH COMM_MSVC_SUPPRESS_WARNING(w)
#else
# define COMM_MSVC_SUPPRESS_WARNING_PUSH
# define COMM_MSVC_SUPPRESS_WARNING(w)
# define COMM_MSVC_SUPPRESS_WARNING_POP
# define COMM_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#endif  // COMMUNICATION_COMMON_COMPILER_FEATURES_H_
