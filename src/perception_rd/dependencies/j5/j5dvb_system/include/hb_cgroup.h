/*************************************************************************
 *                     COPYRIGHT NOTICE
 *            Copyright 2022 Horizon Robotics, Inc.
 *                   All rights reserved.
 *************************************************************************/

#ifndef INC_HB_CGROUP_H_
#define INC_HB_CGROUP_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief cgroup configure
 * @author lianghong.liu
 * @date 28/April/2022
 */
void cgroup_config();

/**
 * @brief set cpu limit
 * @param [in] char* cpu_arg       1 ~ 100
 * @author lianghong.liu
 * @date 28/April/2022
 */
void cpu_limit(char* cpu_arg);

/**
 * @brief set cpu core
 * @param [in] char* cpuset_args       0 ~ 7, related to the number of cpu
 * cores on the system
 * @author lianghong.liu
 * @date 28/April/2022
 */
void cpuset_limit(char* cpuset_args);

/**
 * @brief set memory limit
 * @param [in] char* mem_args       1 ~ 100
 * @author lianghong.liu
 * @date 28/April/2022
 */
void memory_limit(char* mem_args);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif  // INC_HB_CGROUP_H_
