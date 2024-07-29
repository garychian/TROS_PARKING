# Benchmark 使用方法

！！！在CPP中使用
！！！在CPP中使用
！！！在CPP中使用

- 在cpp中引入头文件 `#include "common/benchmark/Benchmark.hpp"`
- 在namespace `senseAD::perception` 内注册一个benchmark组
- 在合适代码里使用，参考下面的方法
- 所有op.cpp都加上后，需要在pipeline.cpp或者其他必经的地方写一个保存benchmark到文件的代码，参考最下面的例子
- 使用ENABLE_BENCHMARK定义进行编译 `ADD_DEFINITIONS(-DENABLE_BENCHMARK)` 

```cpp
namespace senseAD {
namespace perception {

// 这里都是必需代码, 创建一个<Predictor>组的耗时统计，包含<Init> <Process> <XXXX>三个统计项
AD_LOCAL_BENCHMARK_CREATE(Predictor);
// 注册统计项
AD_LOCAL_BENCHMARK_REG(Init);
AD_LOCAL_BENCHMARK_REG(Process);
AD_LOCAL_BENCHMARK_REG(LoopA);
AD_LOCAL_BENCHMARK_REG(XXXXX);

namespace camera {


apStatus_t Predictor::Init(const OperatorParameter& operator_param) {
    AP_LDEBUG(Predictor) << "Init start";
    // 开始统计耗时，使用SCOPE统计这个生命周期内的耗时，统计项是Init，必须在前面有注册
    AD_LOCAL_BENCHMARK_SCOPE_RECORD(Init);

    AP_LDEBUG(Predictor) << "Init successfully.";
    return AP_SUCCESS;
}


apStatus_t CVProcess::Process(Proxy& proxy) {
    AP_LDEBUG(Predictor) << "Process start";
    // 统计整个process耗时，使用SCOPE，跟随生命周期结束
    AD_LOCAL_BENCHMARK_SCOPE_RECORD(Process);

    // 统计整个Loop的耗时，IN OUT一起使用，自定义结束点
    AD_LOCAL_BENCHMARK_RECORD_IN(XXXXX);
    for (auto i : idx) {
        // 统计单次Loop的耗时，使用SCOPE，跟随生命周期结束
        AD_LOCAL_BENCHMARK_SCOPE_RECORD(LoopA);
    }
    // 结束整个Loop的耗时统计
    AD_LOCAL_BENCHMARK_RECORD_OUT(XXXXX);

    AP_LDEBUG(Predictor) << "Process successfully.";
    return AP_SUCCESS;
}

}
}
}
```

```cpp

    // 在pipeline.cpp里面，在process调用里，每500帧保存一次耗时统计
    if (frame_cnt_ % 500 == 0) {
        // 保存到当前路径，以PerceptionCamera为前缀的csv文件里
        senseAD::perception::BenchMarkInstance::GetBenchMarkReport("./PerceptionCamera");
    }

```


