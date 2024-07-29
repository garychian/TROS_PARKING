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

#ifndef SCHEDULEGROUP_UTILS_VISIBILITY_CONTROL_H_
#define SCHEDULEGROUP_UTILS_VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define BOLE_EXPORT __attribute__((dllexport))
#define BOLE_IMPORT __attribute__((dllimport))
#else
#define BOLE_EXPORT __declspec(dllexport)
#define BOLE_IMPORT __declspec(dllimport)
#endif
#ifdef BOLE_BUILDING_LIBRARY
#define BOLE_PUBLIC BOLE_EXPORT
#else
#define BOLE_PUBLIC
#endif
#define BOLE_PUBLIC_TYPE BOLE_PUBLIC
#define BOLE_LOCAL
#else
#define BOLE_EXPORT __attribute__((visibility("default")))
#define BOLE_IMPORT
#if __GNUC__ >= 4
#define BOLE_PUBLIC __attribute__((visibility("default")))
#define BOLE_LOCAL __attribute__((visibility("hidden")))
#else
#define BOLE_PUBLIC
#define BOLE_LOCAL
#endif
#define BOLE_PUBLIC_TYPE
#endif

#endif  // SCHEDULEGROUP_UTILS_VISIBILITY_CONTROL_H_
