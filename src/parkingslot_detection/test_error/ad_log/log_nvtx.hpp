/*
 * Copyright (C) 2017 by SenseTime Group Limited. All rights reserved.
 * Wang Jiadong <wangjiadong@sensetime.com>
 */

#pragma once

#ifdef USE_NVTX

#include <cuda_runtime.h>
#include <nvtx3/nvToolsExt.h>

const uint32_t nvtx_colors[] = {0xff00ff00, 0xff0000ff, 0xffffff00, 0xffff00ff,
                                0xff00ffff, 0xffff0000, 0xffffffff};
const int num_nvtx_colors = sizeof(nvtx_colors) / sizeof(uint32_t);

#define NVTX_PUSH_COLORED(name, cid)                       \
    do {                                                   \
        int color_id = cid;                                \
        color_id = color_id % num_nvtx_colors;             \
        nvtxEventAttributes_t eventAttrib = {0};           \
        eventAttrib.version = NVTX_VERSION;                \
        eventAttrib.size = NVTX_EVENT_ATTRIB_STRUCT_SIZE;  \
        eventAttrib.colorType = NVTX_COLOR_ARGB;           \
        eventAttrib.color = nvtx_colors[color_id];         \
        eventAttrib.messageType = NVTX_MESSAGE_TYPE_ASCII; \
        eventAttrib.message.ascii = #name;                 \
        nvtxRangePushEx(&eventAttrib);                     \
    } while (0)

#define NVTX_PUSH(name) NVTX_PUSH_COLORED(name, 0)

#define NVTX_POP        \
    do {                \
        nvtxRangePop(); \
    } while (0)

namespace senseAD {
class NvtxTracer {
 public:
    explicit NvtxTracer(const char* name) { nvtxRangePushA(name); }
    ~NvtxTracer() { nvtxRangePop(); }
};
}  // namespace senseAD

#define NVTX_RANGE(name) senseAD::NvtxTracer uniq_name_using_macros(#name);

#else  // USE_NVTX

#define NVTX_PUSH_COLORED(name, cid)

#define NVTX_PUSH(name)

#define NVTX_POP

#define NVTX_RANGE(name)

#endif
