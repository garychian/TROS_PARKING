#pragma once

#include <stddef.h>
#include <memory>

namespace senseAD {
namespace swc_embed {
struct DeviceMemory {
    void *context;
    void *device_ptr;
    void *host_ptr;
    size_t size;
};


bool mallocDeviceMemory(const char *strName, size_t size, DeviceMemory *memory);
bool freeDeviceMemory(void *device_ptr, const void *vir_ptr);
bool deviceMemoryMmap(void *device_ptr, void **host_ptr, size_t size);
bool deviceMemoryMunmap(void *host_ptr, size_t size);
struct AllocedDeviceMemory : DeviceMemory {
  AllocedDeviceMemory(const char *strName, size_t size) {
    this->device_ptr = nullptr;
    this->host_ptr = nullptr;
    mallocDeviceMemory(strName, size, this);
  }

  virtual ~AllocedDeviceMemory() {
    if (device_ptr != nullptr) {
        freeDeviceMemory(device_ptr, host_ptr);
        device_ptr = nullptr;
    }
  }
};

struct MapedDeviceMemory : DeviceMemory {
  MapedDeviceMemory(void *device_ptr, size_t size) {
    this->device_ptr = device_ptr;
    this->size = size;
    this->host_ptr = nullptr;
    deviceMemoryMmap(device_ptr, &host_ptr, size);
  }

  virtual ~MapedDeviceMemory() {
    if (host_ptr != nullptr) {
        deviceMemoryMunmap(host_ptr, size);
        host_ptr = nullptr;
    }
  }
};


class DeviceRuntime {
  public:
    static bool Init();

    DeviceRuntime();
    ~DeviceRuntime();

    bool DoInit();
    bool DoDeinit();

  private:
    static std::shared_ptr<DeviceRuntime> instance_;
    bool inited_;
};

}
}
