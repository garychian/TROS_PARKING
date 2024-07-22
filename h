cat: deploy_path: No such file or directory
j5_conan
Release
parking
Configuration:
[settings]
arch=armv8
build_type=Release
compiler=gcc
compiler.libcxx=libstdc++11
compiler.version=9.3
os=Linux
[options]
*:asan_enable=False
*:dataflow=True
*:hbmem=True
*:pagesize=64k
*:soc=j5
[build_requires]
[env]
CC=/opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
CHOST=aarch64-linux-gnu
CONAN_CMAKE_FIND_ROOT_PATH=/opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu
CXX=/opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-g++
STRIP=/opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-strip
[communication][conan] require perfetto
Version ranges solved
    Version range '1.7.x' required by 'conanfile.py (fanya_parking/1.0.0@din/stable)' resolved to 'message/1.7.1@bole/stable' in local cache
    Version range '1.10.x' required by 'dataflow/1.10.5@bole/stable' resolved to 'time/1.10.3@bole/stable' in local cache
    Version range '1.7.x' required by 'communication/1.10.5@bole/stable' valid for downstream requirement 'message/1.7.1@bole/stable'
    Version range '1.10.x' required by 'communication/1.10.5@bole/stable' resolved to 'schedulegroup/1.10.3@bole/stable' in local cache

conanfile.py (fanya_parking/1.0.0@din/stable): Installing package
Requirements
    boost/1.74.0@thirdparty/stable from 'conan-release' - Cache
    cereal/1.3.2@thirdparty/stable from 'conan-release' - Cache
    common/1.6.3@tros/stable from 'conan-release' - Cache
    communication/1.10.5@bole/stable from 'conan-release' - Cache
    convert_assistant/2.5.3@thirdparty/stable from 'conan-test' - Cache
    dataflow/1.10.5@bole/stable from 'conan-release' - Cache
    fanya-protocol/1.0.0@ltc/fanya from local cache - Cache
    filesystem/1.5.12@thirdparty/stable from 'conan-release' - Cache
    fmt/9.0.0@thirdparty/stable from 'conan-release' - Cache
    gperftools/2.10.0@thirdparty/stable from 'conan-release' - Cache
    hlog/1.10.2@bole/stable from 'conan-release' - Cache
    hobot-bole-protocol/1.4.43@bole/stable from 'conan-release' - Cache
    j5dvb_system/2023.10.17@bsp/stable from 'conan-release' - Cache
    jsoncpp/1.9.0@thirdparty/stable from 'conan-release' - Cache
    lzma/5.2.5@thirdparty/stable from 'conan-release' - Cache
    message/1.7.1@bole/stable from 'conan-release' - Cache
    nlohmann_json/3.7.3@thirdparty/stable from 'conan-release' - Cache
    opencv/3.4.5@thirdparty/stable from 'conan-release' - Cache
    openssl/1.1.1a@thirdparty/stable from 'conan-release' - Cache
    perfetto/0.1.3@adas/stable from 'conan-test' - Cache
    protobuf/3.19.3@thirdparty/stable from 'conan-release' - Cache
    schedulegroup/1.10.3@bole/stable from 'conan-release' - Cache
    sensors/1.10.5@bole/stable from 'conan-release' - Cache
    serial/1.2.2@thirdparty/stable from 'conan-release' - Cache
    sm_client/1.6.3@tros/stable from 'conan-release' - Cache
    time/1.10.3@bole/stable from 'conan-release' - Cache
    unwind/1.7.2@thirdparty/stable from 'conan-release' - Cache
    zeromq/4.3.4@thirdparty/stable from 'conan-release' - Cache
Packages
    boost/1.74.0@thirdparty/stable:5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9 - Cache
    cereal/1.3.2@thirdparty/stable:5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9 - Cache
    common/1.6.3@tros/stable:23944d214aefe17a12672c232b98e6aeddd814f3 - Cache
    communication/1.10.5@bole/stable:c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142 - Cache
    convert_assistant/2.5.3@thirdparty/stable:d416bd0169399d6afc0598870a0da6ad51927f6b - Cache
    dataflow/1.10.5@bole/stable:b5dd93bf92834dfae3bcd709778faa7da23b08c1 - Cache
    fanya-protocol/1.0.0@ltc/fanya:6bf24a1075103a426f82a19d5855c39dec869813 - Cache
    filesystem/1.5.12@thirdparty/stable:5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9 - Cache
    fmt/9.0.0@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    gperftools/2.10.0@thirdparty/stable:0e3e4959fbfb7d493172b4e7f7b174dbd246f71f - Cache
    hlog/1.10.2@bole/stable:caa9848281a31a05e0645426d20327e260a2576b - Cache
    hobot-bole-protocol/1.4.43@bole/stable:6bf24a1075103a426f82a19d5855c39dec869813 - Cache
    j5dvb_system/2023.10.17@bsp/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    jsoncpp/1.9.0@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    lzma/5.2.5@thirdparty/stable:10696691b6176bafb8c113c4211cdf3803c0e369 - Cache
    message/1.7.1@bole/stable:5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9 - Cache
    nlohmann_json/3.7.3@thirdparty/stable:5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9 - Cache
    opencv/3.4.5@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    openssl/1.1.1a@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    perfetto/0.1.3@adas/stable:ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c - Cache
    protobuf/3.19.3@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    schedulegroup/1.10.3@bole/stable:27d44ca04b71b4073bb8c15e32bd75fcf4558092 - Cache
    sensors/1.10.5@bole/stable:e9a05079874070994098c936d3131026a67c0c03 - Cache
    serial/1.2.2@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    sm_client/1.6.3@tros/stable:e67c4f625ec1e46b4e3a49f27603b78657dca96f - Cache
    time/1.10.3@bole/stable:46ce5d4fa81ab5be86ef3558f1b294834639d2df - Cache
    unwind/1.7.2@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
    zeromq/4.3.4@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache
Build requirements
    gtest/1.8.0@thirdparty/stable from 'conan-release' - Cache
Build requirements packages
    gtest/1.8.0@thirdparty/stable:70bd41520a018891ab216c5d5bf64db3a66783b6 - Cache

Cross-build from 'Linux:x86_64' to 'Linux:armv8'
Installing (downloading, building) binaries...
boost/1.74.0@thirdparty/stable: Already installed!
cereal/1.3.2@thirdparty/stable: Already installed!
filesystem/1.5.12@thirdparty/stable: Already installed!
fmt/9.0.0@thirdparty/stable: Already installed!
gperftools/2.10.0@thirdparty/stable: Already installed!
gtest/1.8.0@thirdparty/stable: Already installed!
j5dvb_system/2023.10.17@bsp/stable: Already installed!
jsoncpp/1.9.0@thirdparty/stable: Already installed!
lzma/5.2.5@thirdparty/stable: Already installed!
message/1.7.1@bole/stable: Already installed!
nlohmann_json/3.7.3@thirdparty/stable: Already installed!
opencv/3.4.5@thirdparty/stable: Already installed!
openssl/1.1.1a@thirdparty/stable: Already installed!
perfetto/0.1.3@adas/stable: Already installed!
protobuf/3.19.3@thirdparty/stable: Already installed!
serial/1.2.2@thirdparty/stable: Already installed!
unwind/1.7.2@thirdparty/stable: Already installed!
zeromq/4.3.4@thirdparty/stable: Already installed!
convert_assistant/2.5.3@thirdparty/stable: Already installed!
fanya-protocol/1.0.0@ltc/fanya: Already installed!
hlog/1.10.2@bole/stable: Already installed!
hobot-bole-protocol/1.4.43@bole/stable: Already installed!
common/1.6.3@tros/stable: Already installed!
schedulegroup/1.10.3@bole/stable: Already installed!
communication/1.10.5@bole/stable: Already installed!
sm_client/1.6.3@tros/stable: Already installed!
time/1.10.3@bole/stable: Already installed!
dataflow/1.10.5@bole/stable: Already installed!
sensors/1.10.5@bole/stable: Already installed!
conanfile.py (fanya_parking/1.0.0@din/stable): Applying build-requirement: gtest/1.8.0@thirdparty/stable
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findgtest.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findgperftools.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findhobot-bole-protocol.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findsensors.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findconvert_assistant.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findfanya-protocol.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findopencv.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findsm_client.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findserial.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Finddataflow.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findcommon.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findtime.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findopenssl.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findboost.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findcereal.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findfilesystem.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findjsoncpp.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findcommunication.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findprotobuf.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findmessage.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findschedulegroup.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findperfetto.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findnlohmann_json.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findhlog.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findzeromq.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findfmt.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findunwind.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findlzma.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator cmake_find_package created Findj5dvb_system.cmake
conanfile.py (fanya_parking/1.0.0@din/stable): Generator deploy created deploy_manifest.txt
conanfile.py (fanya_parking/1.0.0@din/stable): Generator json created conanbuildinfo.json
conanfile.py (fanya_parking/1.0.0@din/stable): Generator txt created conanbuildinfo.txt
conanfile.py (fanya_parking/1.0.0@din/stable): Aggregating env generators
conanfile.py (fanya_parking/1.0.0@din/stable): Generated conaninfo.txt
conanfile.py (fanya_parking/1.0.0@din/stable): Generated graphinfo
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 5 files
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 2 '.4' files: libopencv_world.so.3.4, libcommunication.so.4
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 16 '.so' files
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 3 '.2' files: libhlog_receiver.so.1.10.2, libconvert_assistant.so.2, libhlog.so.1.10.2
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 9 '.1' files
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 5 '.3' files
conanfile.py (fanya_parking/1.0.0@din/stable) imports(): Copied 4 '.5' files: libsensor_center-share.so.1.10.5, libdataflow.so.1.10.5, libcommunication.so.4.10.5, libsensor_center_common.so.1.10.5
WARN: sensors/1.10.5@bole/stable: requirement sm_client/[1.6.x]@tros/stable overridden by fanya_parking/1.0.0@din/stable to sm_client/1.6.3@tros/stable 
WARN: sensors/1.10.5@bole/stable: requirement dataflow/[1.10.x]@bole/stable overridden by fanya_parking/1.0.0@din/stable to dataflow/1.10.5@bole/stable 
WARN: sm_client/1.6.3@tros/stable: requirement common/[1.6.x]@tros/stable overridden by sensors/1.10.5@bole/stable to common/1.6.3@tros/stable 
WARN: common/1.6.3@tros/stable: requirement hlog/[1.10.x]@bole/stable overridden by sm_client/1.6.3@tros/stable to hlog/1.10.2@bole/stable 
WARN: time/1.10.3@bole/stable: requirement communication/[1.10.x]@bole/stable overridden by dataflow/1.10.5@bole/stable to communication/1.10.5@bole/stable 
WARN: communication/1.10.5@bole/stable: requirement message/[1.7.x]@bole/stable overridden by time/1.10.3@bole/stable to message/1.7.1@bole/stable 
WARN: schedulegroup/1.10.3@bole/stable: requirement hlog/[1.10.x]@bole/stable overridden by communication/1.10.5@bole/stable to hlog/1.10.2@bole/stable 
gperftools/2.10.0@thirdparty/stable: WARN: Library 'tcmalloc_debug' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
gperftools/2.10.0@thirdparty/stable: WARN: Library 'tcmalloc_minimal' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
gperftools/2.10.0@thirdparty/stable: WARN: Library 'tcmalloc' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
gperftools/2.10.0@thirdparty/stable: WARN: Library 'profiler' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
gperftools/2.10.0@thirdparty/stable: WARN: Library 'tcmalloc_minimal_debug' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
gperftools/2.10.0@thirdparty/stable: WARN: Library 'tcmalloc_and_profiler' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'elf' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'ion' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'alog' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'cnn_intf' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'platform_hbm' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'hbcgroup' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'ffmedia' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'hbmem' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'hb_api_isp' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'hbplayer' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'cam' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'secip' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
j5dvb_system/2023.10.17@bsp/stable: WARN: Library 'hbaudio' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
jsoncpp/1.9.0@thirdparty/stable: WARN: Library 'jsoncpp' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
opencv/3.4.5@thirdparty/stable: WARN: Library 'opencv_world' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
openssl/1.1.1a@thirdparty/stable: WARN: Library 'ssl' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
openssl/1.1.1a@thirdparty/stable: WARN: Library 'crypto' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
serial/1.2.2@thirdparty/stable: WARN: Library 'serial' was either already found in a previous 'conanfile.cpp_info.libdirs' folder or appears several times with a different file extension
Using lockfile: '/open_explorer/parking/build/conan.lock'
Using cached profile from lockfile
conanfile.py (fanya_parking/1.0.0@din/stable): Calling build()
-- The CXX compiler identification is GNU 9.3.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Conan: Using autogenerated Findboost.cmake
-- Found boost: 1.74.0 (found version "1.74.0")
find package boost
-- Conan: Using autogenerated Findcereal.cmake
-- Found cereal: 1.3.2 (found version "1.3.2")
find package cereal
-- Conan: Using autogenerated Findcommon.cmake
-- Found common: 1.6.3 (found version "1.6.3")
-- Library common not found in package, might be system one
-- Conan: Using autogenerated Findhlog.cmake
-- Found hlog: 1.10.2 (found version "1.10.2")
-- Library hlog found /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
-- Found: /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
-- Conan: Using autogenerated Findfmt.cmake
-- Found fmt: 9.0.0 (found version "9.0.0")
-- Library fmt found /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
-- Found: /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
-- Conan: Using autogenerated Findzeromq.cmake
-- Found zeromq: 4.3.4 (found version "4.3.4")
-- Library zmq found /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
-- Found: /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
-- Conan: Using autogenerated Findunwind.cmake
-- Found unwind: 1.7.2 (found version "1.7.2")
-- Library unwind found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Library unwind-aarch64 found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Library unwind-coredump found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Library unwind-generic found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Library unwind-ptrace found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Library unwind-setjmp found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
-- Conan: Using autogenerated Findlzma.cmake
-- Found lzma: 5.2.5 (found version "5.2.5")
-- Library lzma found /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
-- Found: /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
-- Conan: Using autogenerated Findj5dvb_system.cmake
-- Found j5dvb_system: 2023.10.17 (found version "2023.10.17")
-- Library BrokenLocale found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Library BrokenLocale-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Library SegFault found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Library a55-stl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Library alog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Library anl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Library anl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Library asan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Library atomic found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Library attr found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Library boardinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Library cam found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Library cap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Library cnn_intf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Library com_err found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Library cryptsetup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Library cutils found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Library diag_rpt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Library dl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Library dl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Library e2p found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Library elf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Library elf-0.177 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Library ext2fs found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Library ffmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Library gdcbin found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Library glog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Library gomp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Library gps found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Library hb_api_isp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Library hbaudio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Library hbcgroup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Library hbmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Library hbmem found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Library hbpcie found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Library hbplayer found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Library hbspihal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Library idu found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Library ion found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Library isp_algo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Library json-c found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Library lkof found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Library lsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Library memusage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Library mnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Library multimedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Library ncurses found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Library nftnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Library nsl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Library nsl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Library nss_hesiod found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Library nss_hesiod-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Library nss_nis found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Library nss_nisplus found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Library pcprofile found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Library perflock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Library persist found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Library platform_hbm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Library popt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Library powerctl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Library rpmsg found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Library rt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Library rt-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Library secip found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Library secure_storage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Library socketcan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Library ss found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Library systrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Library thermal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Library thread_db found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Library thread_db-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Library tinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Library tsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Library ubsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Library usb-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Library usertrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Library util found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Library util-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Library uuid found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Library videostitch found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Library vio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Library vio_client found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Library vpm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Library wakelock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Library wrap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
-- Dependency boost already found
-- Dependency cereal already found
-- Conan: Using autogenerated Findfilesystem.cmake
-- Found filesystem: 1.5.12 (found version "1.5.12")
-- Conan: Using autogenerated Findjsoncpp.cmake
-- Found jsoncpp: 1.9.0 (found version "1.9.0")
-- Library jsoncpp found /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
-- Found: /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
-- Dependency zeromq already found
-- Conan: Using autogenerated Findopenssl.cmake
-- Found openssl: 1.1.1a (found version "1.1.1a")
-- Library crypto found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Library ssl found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
-- Dependency fmt already found
find package common
-- Conan: Using autogenerated Findcommunication.cmake
-- Found communication: 1.10.5 (found version "1.10.5")
-- Library communication found /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/lib/libcommunication.so
-- Found: /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/lib/libcommunication.so
-- Conan: Using autogenerated Findprotobuf.cmake
-- Found protobuf: 3.19.3 (found version "3.19.3")
-- Library protobuf found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Library protobuf-lite found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
-- Dependency zeromq already found
-- Conan: Using autogenerated Findmessage.cmake
-- Found message: 1.7.1 (found version "1.7.1")
-- Conan: Using autogenerated Findschedulegroup.cmake
-- Found schedulegroup: 1.10.3 (found version "1.10.3")
-- Library schedulegroup found /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
-- Found: /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
-- Dependency hlog already found
-- Conan: Using autogenerated Findnlohmann_json.cmake
-- Found nlohmann_json: 3.7.3 (found version "3.7.3")
-- Conan: Using autogenerated Findperfetto.cmake
-- Found perfetto: 0.1.3 (found version "0.1.3")
-- Library perfetto_sdk found /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
-- Found: /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
find package communication
-- Conan: Using autogenerated Findconvert_assistant.cmake
-- Found convert_assistant: 2.5.3 (found version "2.5.3")
-- Library convert_assistant found /root/.conan/data/convert_assistant/2.5.3/thirdparty/stable/package/d416bd0169399d6afc0598870a0da6ad51927f6b/lib/libconvert_assistant.so
-- Found: /root/.conan/data/convert_assistant/2.5.3/thirdparty/stable/package/d416bd0169399d6afc0598870a0da6ad51927f6b/lib/libconvert_assistant.so
-- Conan: Using autogenerated Findopencv.cmake
-- Found opencv: 3.4.5 (found version "3.4.5")
-- Library libjasper found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Library libjpeg-turbo found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Library libpng found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Library libprotobuf found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Library libtiff found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Library libwebp found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Library opencv_world found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Library quirc found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Library tegra_hal found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Library zlib found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
-- Dependency nlohmann_json already found
-- Dependency fmt already found
-- Dependency j5dvb_system already found
find package convert_assistant
-- Conan: Using autogenerated Finddataflow.cmake
-- Found dataflow: 1.10.5 (found version "1.10.5")
-- Library dataflow found /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/lib/libdataflow.so
-- Found: /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/lib/libdataflow.so
-- Conan: Using autogenerated Findtime.cmake
-- Found time: 1.10.3 (found version "1.10.3")
-- Library time found /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
-- Found: /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
-- Dependency communication already found
-- Dependency protobuf already found
find package dataflow
-- Conan: Using autogenerated Findfanya-protocol.cmake
-- Found fanya-protocol: 1.0.0 (found version "1.0.0")
-- Library fanya-protocol not found in package, might be system one
-- Dependency protobuf already found
find package fanya-protocol
-- Conan: Using autogenerated Findfilesystem.cmake
find package filesystem
-- Conan: Using autogenerated Findfmt.cmake
-- Library fmt found /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
-- Skipping already existing target: CONAN_LIB::fmt_fmt
-- Found: /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
find package fmt
-- Conan: Using autogenerated Findgperftools.cmake
-- Found gperftools: 2.10.0 (found version "2.10.0")
-- Library profiler found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libprofiler.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libprofiler.so
-- Library tcmalloc found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc.so
-- Library tcmalloc_and_profiler found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_and_profiler.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_and_profiler.so
-- Library tcmalloc_debug found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_debug.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_debug.so
-- Library tcmalloc_minimal found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal.so
-- Library tcmalloc_minimal_debug found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal_debug.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal_debug.so
find package gperftools
-- Conan: Using autogenerated Findgtest.cmake
-- Found gtest: 1.8.0 (found version "1.8.0")
-- Library gtest found /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest.a
-- Found: /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest.a
-- Library gtest_main found /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest_main.a
-- Found: /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest_main.a
find package gtest
-- Conan: Using autogenerated Findhlog.cmake
-- Library hlog found /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
-- Skipping already existing target: CONAN_LIB::hlog_hlog
-- Found: /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
find package hlog
-- Conan: Using autogenerated Findhobot-bole-protocol.cmake
-- Found hobot-bole-protocol: 1.4.43 (found version "1.4.43")
-- Library hobot-bole-protocol found /root/.conan/data/hobot-bole-protocol/1.4.43/bole/stable/package/6bf24a1075103a426f82a19d5855c39dec869813/lib/libhobot-bole-protocol.a
-- Found: /root/.conan/data/hobot-bole-protocol/1.4.43/bole/stable/package/6bf24a1075103a426f82a19d5855c39dec869813/lib/libhobot-bole-protocol.a
-- Dependency protobuf already found
find package hobot-bole-protocol
-- Conan: Using autogenerated Findj5dvb_system.cmake
-- Library BrokenLocale found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_BrokenLocale
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Library BrokenLocale-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_BrokenLocale-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Library SegFault found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_SegFault
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Library a55-stl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_a55-stl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Library alog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_alog
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Library anl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_anl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Library anl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_anl-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Library asan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_asan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Library atomic found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_atomic
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Library attr found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_attr
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Library boardinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_boardinfo
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Library cam found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cam
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Library cap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cap
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Library cnn_intf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cnn_intf
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Library com_err found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_com_err
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Library cryptsetup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cryptsetup
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Library cutils found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cutils
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Library diag_rpt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_diag_rpt
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Library dl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_dl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Library dl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_dl-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Library e2p found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_e2p
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Library elf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_elf
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Library elf-0.177 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_elf-0.177
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Library ext2fs found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ext2fs
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Library ffmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ffmedia
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Library gdcbin found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_gdcbin
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Library glog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_glog
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Library gomp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_gomp
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Library gps found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_gps
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Library hb_api_isp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hb_api_isp
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Library hbaudio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbaudio
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Library hbcgroup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbcgroup
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Library hbmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbmedia
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Library hbmem found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbmem
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Library hbpcie found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbpcie
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Library hbplayer found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbplayer
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Library hbspihal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbspihal
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Library idu found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_idu
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Library ion found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ion
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Library isp_algo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_isp_algo
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Library json-c found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_json-c
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Library lkof found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_lkof
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Library lsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_lsan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Library memusage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_memusage
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Library mnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_mnl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Library multimedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_multimedia
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Library ncurses found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ncurses
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Library nftnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nftnl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Library nsl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nsl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Library nsl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nsl-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Library nss_hesiod found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_hesiod
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Library nss_hesiod-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_hesiod-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Library nss_nis found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_nis
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Library nss_nisplus found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_nisplus
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Library pcprofile found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_pcprofile
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Library perflock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_perflock
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Library persist found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_persist
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Library platform_hbm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_platform_hbm
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Library popt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_popt
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Library powerctl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_powerctl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Library rpmsg found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_rpmsg
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Library rt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_rt
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Library rt-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_rt-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Library secip found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_secip
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Library secure_storage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_secure_storage
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Library socketcan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_socketcan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Library ss found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ss
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Library systrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_systrace
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Library thermal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_thermal
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Library thread_db found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_thread_db
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Library thread_db-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_thread_db-1.0
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Library tinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_tinfo
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Library tsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_tsan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Library ubsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ubsan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Library usb-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_usb-1.0
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Library usertrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_usertrace
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Library util found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_util
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Library util-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_util-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Library uuid found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_uuid
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Library videostitch found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_videostitch
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Library vio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_vio
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Library vio_client found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_vio_client
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Library vpm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_vpm
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Library wakelock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_wakelock
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Library wrap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_wrap
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
find package j5dvb_system
-- Conan: Using autogenerated Findjsoncpp.cmake
-- Library jsoncpp found /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
-- Skipping already existing target: CONAN_LIB::jsoncpp_jsoncpp
-- Found: /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
find package jsoncpp
-- Conan: Using autogenerated Findlzma.cmake
-- Library lzma found /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
-- Skipping already existing target: CONAN_LIB::lzma_lzma
-- Found: /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
find package lzma
-- Conan: Using autogenerated Findmessage.cmake
find package message
-- Conan: Using autogenerated Findnlohmann_json.cmake
find package nlohmann_json
-- Conan: Using autogenerated Findopencv.cmake
-- Library libjasper found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Skipping already existing target: CONAN_LIB::opencv_libjasper
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Library libjpeg-turbo found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Skipping already existing target: CONAN_LIB::opencv_libjpeg-turbo
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Library libpng found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Skipping already existing target: CONAN_LIB::opencv_libpng
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Library libprotobuf found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Skipping already existing target: CONAN_LIB::opencv_libprotobuf
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Library libtiff found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Skipping already existing target: CONAN_LIB::opencv_libtiff
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Library libwebp found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Skipping already existing target: CONAN_LIB::opencv_libwebp
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Library opencv_world found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Skipping already existing target: CONAN_LIB::opencv_opencv_world
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Library quirc found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Skipping already existing target: CONAN_LIB::opencv_quirc
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Library tegra_hal found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Skipping already existing target: CONAN_LIB::opencv_tegra_hal
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Library zlib found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
-- Skipping already existing target: CONAN_LIB::opencv_zlib
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
find package opencv
-- Conan: Using autogenerated Findopenssl.cmake
-- Library crypto found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Skipping already existing target: CONAN_LIB::openssl_crypto
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Library ssl found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
-- Skipping already existing target: CONAN_LIB::openssl_ssl
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
find package openssl
-- Conan: Using autogenerated Findperfetto.cmake
-- Library perfetto_sdk found /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
-- Skipping already existing target: CONAN_LIB::perfetto_perfetto_sdk
-- Found: /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
find package perfetto
-- Conan: Using autogenerated Findprotobuf.cmake
-- Library protobuf found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Skipping already existing target: CONAN_LIB::protobuf_protobuf
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Library protobuf-lite found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
-- Skipping already existing target: CONAN_LIB::protobuf_protobuf-lite
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
find package protobuf
-- Conan: Using autogenerated Findschedulegroup.cmake
-- Library schedulegroup found /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
-- Skipping already existing target: CONAN_LIB::schedulegroup_schedulegroup
-- Found: /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
find package schedulegroup
-- Conan: Using autogenerated Findsensors.cmake
-- Found sensors: 1.10.5 (found version "1.10.5")
-- Library sensors not found in package, might be system one
-- Dependency openssl already found
-- Conan: Using autogenerated Findsm_client.cmake
-- Found sm_client: 1.6.3 (found version "1.6.3")
-- Library sm_client found /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
-- Found: /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
-- Dependency boost already found
-- Dependency cereal already found
-- Dependency filesystem already found
-- Dependency jsoncpp already found
-- Dependency zeromq already found
-- Dependency openssl already found
-- Dependency fmt already found
-- Dependency common already found
-- Conan: Using autogenerated Findserial.cmake
-- Found serial: 1.2.2 (found version "1.2.2")
-- Library serial found /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
-- Found: /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
-- Dependency dataflow already found
find package sensors
-- Conan: Using autogenerated Findserial.cmake
-- Library serial found /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
-- Skipping already existing target: CONAN_LIB::serial_serial
-- Found: /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
find package serial
-- Conan: Using autogenerated Findsm_client.cmake
-- Library sm_client found /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
-- Skipping already existing target: CONAN_LIB::sm_client_sm_client
-- Found: /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
find package sm_client
-- Conan: Using autogenerated Findtime.cmake
-- Library time found /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
-- Skipping already existing target: CONAN_LIB::time_time
-- Found: /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
find package time
-- Conan: Using autogenerated Findunwind.cmake
-- Library unwind found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Library unwind-aarch64 found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-aarch64
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Library unwind-coredump found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-coredump
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Library unwind-generic found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-generic
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Library unwind-ptrace found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-ptrace
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Library unwind-setjmp found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-setjmp
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
find package unwind
-- Conan: Using autogenerated Findzeromq.cmake
-- Library zmq found /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
-- Skipping already existing target: CONAN_LIB::zeromq_zmq
-- Found: /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
find package zeromq
-- Configuring done (0.4s)
-- Generating done (0.0s)
CMake Warning:
  Manually-specified variables were not used by the project:

    ARCH
    ARM
    CMAKE_EXPORT_NO_PACKAGE_REGISTRY
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_DATAROOTDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
    CMAKE_INSTALL_LIBEXECDIR
    CMAKE_INSTALL_OLDINCLUDEDIR
    CMAKE_INSTALL_SBINDIR
    CONAN_CMAKE_FIND_ROOT_PATH
    CONAN_COMPILER
    CONAN_COMPILER_VERSION
    CONAN_EXPORTED
    CONAN_IN_LOCAL_CACHE
    CONAN_LIBCXX
    OS
    VERSION_MAJOR
    VERSION_MINOR
    VERSION_PATCH


-- Build files have been written to: /open_explorer/parking/build/build_subfolder
-- The CXX compiler identification is GNU 9.3.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Conan: Using autogenerated Findboost.cmake
-- Found boost: 1.74.0 (found version "1.74.0")
find package boost
-- Conan: Using autogenerated Findcereal.cmake
-- Found cereal: 1.3.2 (found version "1.3.2")
find package cereal
-- Conan: Using autogenerated Findcommon.cmake
-- Found common: 1.6.3 (found version "1.6.3")
-- Library common not found in package, might be system one
-- Conan: Using autogenerated Findhlog.cmake
-- Found hlog: 1.10.2 (found version "1.10.2")
-- Library hlog found /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
-- Found: /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
-- Conan: Using autogenerated Findfmt.cmake
-- Found fmt: 9.0.0 (found version "9.0.0")
-- Library fmt found /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
-- Found: /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
-- Conan: Using autogenerated Findzeromq.cmake
-- Found zeromq: 4.3.4 (found version "4.3.4")
-- Library zmq found /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
-- Found: /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
-- Conan: Using autogenerated Findunwind.cmake
-- Found unwind: 1.7.2 (found version "1.7.2")
-- Library unwind found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Library unwind-aarch64 found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Library unwind-coredump found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Library unwind-generic found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Library unwind-ptrace found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Library unwind-setjmp found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
-- Conan: Using autogenerated Findlzma.cmake
-- Found lzma: 5.2.5 (found version "5.2.5")
-- Library lzma found /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
-- Found: /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
-- Conan: Using autogenerated Findj5dvb_system.cmake
-- Found j5dvb_system: 2023.10.17 (found version "2023.10.17")
-- Library BrokenLocale found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Library BrokenLocale-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Library SegFault found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Library a55-stl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Library alog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Library anl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Library anl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Library asan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Library atomic found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Library attr found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Library boardinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Library cam found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Library cap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Library cnn_intf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Library com_err found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Library cryptsetup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Library cutils found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Library diag_rpt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Library dl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Library dl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Library e2p found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Library elf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Library elf-0.177 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Library ext2fs found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Library ffmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Library gdcbin found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Library glog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Library gomp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Library gps found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Library hb_api_isp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Library hbaudio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Library hbcgroup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Library hbmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Library hbmem found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Library hbpcie found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Library hbplayer found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Library hbspihal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Library idu found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Library ion found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Library isp_algo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Library json-c found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Library lkof found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Library lsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Library memusage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Library mnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Library multimedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Library ncurses found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Library nftnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Library nsl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Library nsl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Library nss_hesiod found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Library nss_hesiod-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Library nss_nis found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Library nss_nisplus found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Library pcprofile found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Library perflock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Library persist found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Library platform_hbm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Library popt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Library powerctl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Library rpmsg found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Library rt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Library rt-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Library secip found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Library secure_storage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Library socketcan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Library ss found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Library systrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Library thermal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Library thread_db found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Library thread_db-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Library tinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Library tsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Library ubsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Library usb-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Library usertrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Library util found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Library util-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Library uuid found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Library videostitch found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Library vio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Library vio_client found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Library vpm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Library wakelock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Library wrap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
-- Dependency boost already found
-- Dependency cereal already found
-- Conan: Using autogenerated Findfilesystem.cmake
-- Found filesystem: 1.5.12 (found version "1.5.12")
-- Conan: Using autogenerated Findjsoncpp.cmake
-- Found jsoncpp: 1.9.0 (found version "1.9.0")
-- Library jsoncpp found /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
-- Found: /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
-- Dependency zeromq already found
-- Conan: Using autogenerated Findopenssl.cmake
-- Found openssl: 1.1.1a (found version "1.1.1a")
-- Library crypto found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Library ssl found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
-- Dependency fmt already found
find package common
-- Conan: Using autogenerated Findcommunication.cmake
-- Found communication: 1.10.5 (found version "1.10.5")
-- Library communication found /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/lib/libcommunication.so
-- Found: /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/lib/libcommunication.so
-- Conan: Using autogenerated Findprotobuf.cmake
-- Found protobuf: 3.19.3 (found version "3.19.3")
-- Library protobuf found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Library protobuf-lite found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
-- Dependency zeromq already found
-- Conan: Using autogenerated Findmessage.cmake
-- Found message: 1.7.1 (found version "1.7.1")
-- Conan: Using autogenerated Findschedulegroup.cmake
-- Found schedulegroup: 1.10.3 (found version "1.10.3")
-- Library schedulegroup found /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
-- Found: /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
-- Dependency hlog already found
-- Conan: Using autogenerated Findnlohmann_json.cmake
-- Found nlohmann_json: 3.7.3 (found version "3.7.3")
-- Conan: Using autogenerated Findperfetto.cmake
-- Found perfetto: 0.1.3 (found version "0.1.3")
-- Library perfetto_sdk found /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
-- Found: /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
find package communication
-- Conan: Using autogenerated Findconvert_assistant.cmake
-- Found convert_assistant: 2.5.3 (found version "2.5.3")
-- Library convert_assistant found /root/.conan/data/convert_assistant/2.5.3/thirdparty/stable/package/d416bd0169399d6afc0598870a0da6ad51927f6b/lib/libconvert_assistant.so
-- Found: /root/.conan/data/convert_assistant/2.5.3/thirdparty/stable/package/d416bd0169399d6afc0598870a0da6ad51927f6b/lib/libconvert_assistant.so
-- Conan: Using autogenerated Findopencv.cmake
-- Found opencv: 3.4.5 (found version "3.4.5")
-- Library libjasper found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Library libjpeg-turbo found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Library libpng found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Library libprotobuf found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Library libtiff found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Library libwebp found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Library opencv_world found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Library quirc found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Library tegra_hal found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Library zlib found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
-- Dependency nlohmann_json already found
-- Dependency fmt already found
-- Dependency j5dvb_system already found
find package convert_assistant
-- Conan: Using autogenerated Finddataflow.cmake
-- Found dataflow: 1.10.5 (found version "1.10.5")
-- Library dataflow found /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/lib/libdataflow.so
-- Found: /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/lib/libdataflow.so
-- Conan: Using autogenerated Findtime.cmake
-- Found time: 1.10.3 (found version "1.10.3")
-- Library time found /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
-- Found: /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
-- Dependency communication already found
-- Dependency protobuf already found
find package dataflow
-- Conan: Using autogenerated Findfanya-protocol.cmake
-- Found fanya-protocol: 1.0.0 (found version "1.0.0")
-- Library fanya-protocol not found in package, might be system one
-- Dependency protobuf already found
find package fanya-protocol
-- Conan: Using autogenerated Findfilesystem.cmake
find package filesystem
-- Conan: Using autogenerated Findfmt.cmake
-- Library fmt found /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
-- Skipping already existing target: CONAN_LIB::fmt_fmt
-- Found: /root/.conan/data/fmt/9.0.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libfmt.a
find package fmt
-- Conan: Using autogenerated Findgperftools.cmake
-- Found gperftools: 2.10.0 (found version "2.10.0")
-- Library profiler found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libprofiler.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libprofiler.so
-- Library tcmalloc found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc.so
-- Library tcmalloc_and_profiler found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_and_profiler.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_and_profiler.so
-- Library tcmalloc_debug found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_debug.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_debug.so
-- Library tcmalloc_minimal found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal.so
-- Library tcmalloc_minimal_debug found /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal_debug.so
-- Found: /root/.conan/data/gperftools/2.10.0/thirdparty/stable/package/0e3e4959fbfb7d493172b4e7f7b174dbd246f71f/lib/libtcmalloc_minimal_debug.so
find package gperftools
-- Conan: Using autogenerated Findgtest.cmake
-- Found gtest: 1.8.0 (found version "1.8.0")
-- Library gtest found /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest.a
-- Found: /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest.a
-- Library gtest_main found /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest_main.a
-- Found: /root/.conan/data/gtest/1.8.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgtest_main.a
find package gtest
-- Conan: Using autogenerated Findhlog.cmake
-- Library hlog found /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
-- Skipping already existing target: CONAN_LIB::hlog_hlog
-- Found: /root/.conan/data/hlog/1.10.2/bole/stable/package/caa9848281a31a05e0645426d20327e260a2576b/lib/libhlog.so
find package hlog
-- Conan: Using autogenerated Findhobot-bole-protocol.cmake
-- Found hobot-bole-protocol: 1.4.43 (found version "1.4.43")
-- Library hobot-bole-protocol found /root/.conan/data/hobot-bole-protocol/1.4.43/bole/stable/package/6bf24a1075103a426f82a19d5855c39dec869813/lib/libhobot-bole-protocol.a
-- Found: /root/.conan/data/hobot-bole-protocol/1.4.43/bole/stable/package/6bf24a1075103a426f82a19d5855c39dec869813/lib/libhobot-bole-protocol.a
-- Dependency protobuf already found
find package hobot-bole-protocol
-- Conan: Using autogenerated Findj5dvb_system.cmake
-- Library BrokenLocale found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_BrokenLocale
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale.so
-- Library BrokenLocale-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_BrokenLocale-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libBrokenLocale-2.31.so
-- Library SegFault found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_SegFault
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libSegFault.so
-- Library a55-stl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_a55-stl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liba55-stl.so
-- Library alog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_alog
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libalog.so
-- Library anl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_anl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl.so
-- Library anl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_anl-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libanl-2.31.so
-- Library asan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_asan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libasan.so
-- Library atomic found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_atomic
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libatomic.so
-- Library attr found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_attr
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libattr.so
-- Library boardinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_boardinfo
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libboardinfo.so
-- Library cam found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cam
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcam.so
-- Library cap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cap
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcap.so
-- Library cnn_intf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cnn_intf
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcnn_intf.so
-- Library com_err found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_com_err
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcom_err.so
-- Library cryptsetup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cryptsetup
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcryptsetup.so
-- Library cutils found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_cutils
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcutils.so
-- Library diag_rpt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_diag_rpt
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdiag_rpt.so
-- Library dl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_dl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl.so
-- Library dl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_dl-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libdl-2.31.so
-- Library e2p found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_e2p
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libe2p.so
-- Library elf found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_elf
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf.so
-- Library elf-0.177 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_elf-0.177
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libelf-0.177.so
-- Library ext2fs found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ext2fs
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libext2fs.so
-- Library ffmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ffmedia
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libffmedia.so
-- Library gdcbin found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_gdcbin
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgdcbin.so
-- Library glog found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_glog
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libglog.so
-- Library gomp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_gomp
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgomp.so
-- Library gps found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_gps
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libgps.so
-- Library hb_api_isp found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hb_api_isp
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhb_api_isp.so
-- Library hbaudio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbaudio
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbaudio.so
-- Library hbcgroup found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbcgroup
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbcgroup.so
-- Library hbmedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbmedia
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmedia.so
-- Library hbmem found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbmem
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbmem.so
-- Library hbpcie found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbpcie
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbpcie.so
-- Library hbplayer found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbplayer
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbplayer.so
-- Library hbspihal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_hbspihal
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libhbspihal.so
-- Library idu found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_idu
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libidu.so
-- Library ion found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ion
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libion.so
-- Library isp_algo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_isp_algo
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libisp_algo.so
-- Library json-c found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_json-c
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjson-c.so
-- Library lkof found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_lkof
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblkof.so
-- Library lsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_lsan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblsan.so
-- Library memusage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_memusage
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmemusage.so
-- Library mnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_mnl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmnl.so
-- Library multimedia found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_multimedia
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libmultimedia.so
-- Library ncurses found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ncurses
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libncurses.so
-- Library nftnl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nftnl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnftnl.so
-- Library nsl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nsl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl.so
-- Library nsl-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nsl-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnsl-2.31.so
-- Library nss_hesiod found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_hesiod
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod.so
-- Library nss_hesiod-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_hesiod-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_hesiod-2.31.so
-- Library nss_nis found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_nis
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nis.so
-- Library nss_nisplus found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_nss_nisplus
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libnss_nisplus.so
-- Library pcprofile found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_pcprofile
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpcprofile.so
-- Library perflock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_perflock
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libperflock.so
-- Library persist found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_persist
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpersist.so
-- Library platform_hbm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_platform_hbm
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libplatform_hbm.so
-- Library popt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_popt
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpopt.so
-- Library powerctl found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_powerctl
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libpowerctl.so
-- Library rpmsg found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_rpmsg
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librpmsg.so
-- Library rt found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_rt
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt.so
-- Library rt-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_rt-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/librt-2.31.so
-- Library secip found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_secip
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecip.so
-- Library secure_storage found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_secure_storage
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsecure_storage.so
-- Library socketcan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_socketcan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsocketcan.so
-- Library ss found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ss
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libss.so
-- Library systrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_systrace
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libsystrace.so
-- Library thermal found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_thermal
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthermal.so
-- Library thread_db found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_thread_db
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db.so
-- Library thread_db-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_thread_db-1.0
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libthread_db-1.0.so
-- Library tinfo found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_tinfo
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtinfo.so
-- Library tsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_tsan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtsan.so
-- Library ubsan found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_ubsan
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libubsan.so
-- Library usb-1.0 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_usb-1.0
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusb-1.0.so
-- Library usertrace found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_usertrace
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libusertrace.so
-- Library util found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_util
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil.so
-- Library util-2.31 found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_util-2.31
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libutil-2.31.so
-- Library uuid found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_uuid
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libuuid.so
-- Library videostitch found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_videostitch
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvideostitch.so
-- Library vio found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_vio
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio.so
-- Library vio_client found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_vio_client
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvio_client.so
-- Library vpm found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_vpm
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libvpm.so
-- Library wakelock found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_wakelock
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwakelock.so
-- Library wrap found /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
-- Skipping already existing target: CONAN_LIB::j5dvb_system_wrap
-- Found: /root/.conan/data/j5dvb_system/2023.10.17/bsp/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libwrap.so
find package j5dvb_system
-- Conan: Using autogenerated Findjsoncpp.cmake
-- Library jsoncpp found /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
-- Skipping already existing target: CONAN_LIB::jsoncpp_jsoncpp
-- Found: /root/.conan/data/jsoncpp/1.9.0/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libjsoncpp.so
find package jsoncpp
-- Conan: Using autogenerated Findlzma.cmake
-- Library lzma found /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
-- Skipping already existing target: CONAN_LIB::lzma_lzma
-- Found: /root/.conan/data/lzma/5.2.5/thirdparty/stable/package/10696691b6176bafb8c113c4211cdf3803c0e369/lib/liblzma.a
find package lzma
-- Conan: Using autogenerated Findmessage.cmake
find package message
-- Conan: Using autogenerated Findnlohmann_json.cmake
find package nlohmann_json
-- Conan: Using autogenerated Findopencv.cmake
-- Library libjasper found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Skipping already existing target: CONAN_LIB::opencv_libjasper
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjasper.a
-- Library libjpeg-turbo found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Skipping already existing target: CONAN_LIB::opencv_libjpeg-turbo
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibjpeg-turbo.a
-- Library libpng found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Skipping already existing target: CONAN_LIB::opencv_libpng
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibpng.a
-- Library libprotobuf found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Skipping already existing target: CONAN_LIB::opencv_libprotobuf
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibprotobuf.a
-- Library libtiff found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Skipping already existing target: CONAN_LIB::opencv_libtiff
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibtiff.a
-- Library libwebp found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Skipping already existing target: CONAN_LIB::opencv_libwebp
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/liblibwebp.a
-- Library opencv_world found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Skipping already existing target: CONAN_LIB::opencv_opencv_world
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libopencv_world.so
-- Library quirc found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Skipping already existing target: CONAN_LIB::opencv_quirc
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libquirc.a
-- Library tegra_hal found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Skipping already existing target: CONAN_LIB::opencv_tegra_hal
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libtegra_hal.a
-- Library zlib found /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
-- Skipping already existing target: CONAN_LIB::opencv_zlib
-- Found: /root/.conan/data/opencv/3.4.5/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzlib.a
find package opencv
-- Conan: Using autogenerated Findopenssl.cmake
-- Library crypto found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Skipping already existing target: CONAN_LIB::openssl_crypto
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libcrypto.so
-- Library ssl found /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
-- Skipping already existing target: CONAN_LIB::openssl_ssl
-- Found: /root/.conan/data/openssl/1.1.1a/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libssl.so
find package openssl
-- Conan: Using autogenerated Findperfetto.cmake
-- Library perfetto_sdk found /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
-- Skipping already existing target: CONAN_LIB::perfetto_perfetto_sdk
-- Found: /root/.conan/data/perfetto/0.1.3/adas/stable/package/ddd60ad49a8fcce3aad5eccaea00ddf08c9c845c/lib/libperfetto_sdk.so
find package perfetto
-- Conan: Using autogenerated Findprotobuf.cmake
-- Library protobuf found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Skipping already existing target: CONAN_LIB::protobuf_protobuf
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf.a
-- Library protobuf-lite found /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
-- Skipping already existing target: CONAN_LIB::protobuf_protobuf-lite
-- Found: /root/.conan/data/protobuf/3.19.3/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libprotobuf-lite.a
find package protobuf
-- Conan: Using autogenerated Findschedulegroup.cmake
-- Library schedulegroup found /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
-- Skipping already existing target: CONAN_LIB::schedulegroup_schedulegroup
-- Found: /root/.conan/data/schedulegroup/1.10.3/bole/stable/package/27d44ca04b71b4073bb8c15e32bd75fcf4558092/lib/libschedulegroup.so
find package schedulegroup
-- Conan: Using autogenerated Findsensors.cmake
-- Found sensors: 1.10.5 (found version "1.10.5")
-- Library sensors not found in package, might be system one
-- Dependency openssl already found
-- Conan: Using autogenerated Findsm_client.cmake
-- Found sm_client: 1.6.3 (found version "1.6.3")
-- Library sm_client found /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
-- Found: /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
-- Dependency boost already found
-- Dependency cereal already found
-- Dependency filesystem already found
-- Dependency jsoncpp already found
-- Dependency zeromq already found
-- Dependency openssl already found
-- Dependency fmt already found
-- Dependency common already found
-- Conan: Using autogenerated Findserial.cmake
-- Found serial: 1.2.2 (found version "1.2.2")
-- Library serial found /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
-- Found: /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
-- Dependency dataflow already found
find package sensors
-- Conan: Using autogenerated Findserial.cmake
-- Library serial found /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
-- Skipping already existing target: CONAN_LIB::serial_serial
-- Found: /root/.conan/data/serial/1.2.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libserial.so
find package serial
-- Conan: Using autogenerated Findsm_client.cmake
-- Library sm_client found /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
-- Skipping already existing target: CONAN_LIB::sm_client_sm_client
-- Found: /root/.conan/data/sm_client/1.6.3/tros/stable/package/e67c4f625ec1e46b4e3a49f27603b78657dca96f/lib/libsm_client.so
find package sm_client
-- Conan: Using autogenerated Findtime.cmake
-- Library time found /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
-- Skipping already existing target: CONAN_LIB::time_time
-- Found: /root/.conan/data/time/1.10.3/bole/stable/package/46ce5d4fa81ab5be86ef3558f1b294834639d2df/lib/libtime.so
find package time
-- Conan: Using autogenerated Findunwind.cmake
-- Library unwind found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind.a
-- Library unwind-aarch64 found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-aarch64
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-aarch64.a
-- Library unwind-coredump found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-coredump
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-coredump.a
-- Library unwind-generic found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-generic
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-generic.a
-- Library unwind-ptrace found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-ptrace
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-ptrace.a
-- Library unwind-setjmp found /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
-- Skipping already existing target: CONAN_LIB::unwind_unwind-setjmp
-- Found: /root/.conan/data/unwind/1.7.2/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libunwind-setjmp.a
find package unwind
-- Conan: Using autogenerated Findzeromq.cmake
-- Library zmq found /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
-- Skipping already existing target: CONAN_LIB::zeromq_zmq
-- Found: /root/.conan/data/zeromq/4.3.4/thirdparty/stable/package/70bd41520a018891ab216c5d5bf64db3a66783b6/lib/libzmq.a
find package zeromq
-- Configuring done (0.4s)
-- Generating done (0.0s)
CMake Warning:
  Manually-specified variables were not used by the project:

    ARCH
    ARM
    CMAKE_EXPORT_NO_PACKAGE_REGISTRY
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_DATAROOTDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
    CMAKE_INSTALL_LIBEXECDIR
    CMAKE_INSTALL_OLDINCLUDEDIR
    CMAKE_INSTALL_SBINDIR
    CONAN_CMAKE_FIND_ROOT_PATH
    CONAN_COMPILER
    CONAN_COMPILER_VERSION
    CONAN_EXPORTED
    CONAN_IN_LOCAL_CACHE
    CONAN_LIBCXX
    OS
    VERSION_MAJOR
    VERSION_MINOR
    VERSION_PATCH


-- Build files have been written to: /open_explorer/parking/build
[ 10%] Building CXX object src/parkingslot_detection/CMakeFiles/psd.dir/parkingslot_detect_moudle.cpp.o
[ 40%] Building CXX object src/ipm_transformation/CMakeFiles/ipm_trans.dir/ipm_tranformation_module.cpp.o
[ 40%] Building CXX object src/ipm_transformation/CMakeFiles/ipm_trans.dir/__/common/utils.cpp.o
[ 40%] Building CXX object src/location_map/CMakeFiles/loc.dir/location_map_module.cpp.o
[ 50%] Building CXX object src/apa_handle/CMakeFiles/aph.dir/apa_handle_module.cpp.o
[ 60%] Building CXX object src/parkingslot_detection/CMakeFiles/psd.dir/parkingslot_detect_moudle_old.cpp.o
In file included from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/subscriber/subscriber_wrapper.h:24,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/subscriber/subscriber_manager.h:25,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/node.h:56,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/module/module.h:36,
                 from /open_explorer/parking/include/parkingslot_detection/parkingslot_detect_moudle.h:17,
                 from /open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:13:
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h: In instantiation of 'class hobot::communication::Subscriber<hobot::message::ProtoMsg<simple_proto::SimpleImage> >':
/open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:359:45:   required from here
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h:30:9: error: no type named 'MsgType' in 'struct hobot::message::ProtoMsg<simple_proto::SimpleImage>'
   30 |   using SpMessage = std::shared_ptr<typename Serializer::MsgType>;
      |         ^~~~~~~~~
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h:31:9: error: no type named 'MsgType' in 'struct hobot::message::ProtoMsg<simple_proto::SimpleImage>'
   31 |   using HistoryMsgType = std::pair<SpMessage, std::string>;
      |         ^~~~~~~~~~~~~~
In file included from /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/common/types.h:21,
                 from /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/common/action_types.h:24,
                 from /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/action/action_client_impl.h:24,
                 from /root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/action_client.h:22,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/callback/callback.h:24,
                 from /open_explorer/parking/include/parkingslot_detection/parkingslot_detect_moudle.h:16,
                 from /open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:13:
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/core_types.h: In substitution of 'template<class Serializer> using SubscriberCallback = std::function<void(const std::shared_ptr<typename Serializer::MsgType>&)> [with Serializer = hobot::message::ProtoMsg<simple_proto::SimpleImage>]':
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h:79:41:   required from 'class hobot::communication::Subscriber<hobot::message::ProtoMsg<simple_proto::SimpleImage> >'
/open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:359:45:   required from here
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/core_types.h:359:7: error: no type named 'MsgType' in 'struct hobot::message::ProtoMsg<simple_proto::SimpleImage>'
  359 | using SubscriberCallback =
      |       ^~~~~~~~~~~~~~~~~~
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/core_types.h: In substitution of 'template<class Serializer> using SubscriberWithTopicCallback = std::function<void(const std::shared_ptr<typename Serializer::MsgType>&, const std::__cxx11::basic_string<char>&)> [with Serializer = hobot::message::ProtoMsg<simple_proto::SimpleImage>]':
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h:134:1:   required from 'class hobot::communication::Subscriber<hobot::message::ProtoMsg<simple_proto::SimpleImage> >'
/open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:359:45:   required from here
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/core_types.h:368:7: error: no type named 'MsgType' in 'struct hobot::message::ProtoMsg<simple_proto::SimpleImage>'
  368 | using SubscriberWithTopicCallback =
      |       ^~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/subscriber/subscriber_wrapper.h:24,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/subscriber/subscriber_manager.h:25,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/node.h:56,
                 from /root/.conan/data/dataflow/1.10.5/bole/stable/package/b5dd93bf92834dfae3bcd709778faa7da23b08c1/include/dataflow/module/module.h:36,
                 from /open_explorer/parking/include/parkingslot_detection/parkingslot_detect_moudle.h:17,
                 from /open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:13:
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h: In instantiation of 'class hobot::communication::Subscriber<hobot::message::ProtoMsg<simple_proto::SimpleImage> >':
/open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:359:45:   required from here
/root/.conan/data/communication/1.10.5/bole/stable/package/c7cb13cd6a02f6e5b3fc93b55fb24eca03b1c142/include/communication/subscriber.h:72:51: error: no type named 'MsgType' in 'struct hobot::message::ProtoMsg<simple_proto::SimpleImage>'
   72 |   FixedSizeQueue<std::shared_ptr<HistoryMsgType>> history_queue_;
      |                                                   ^~~~~~~~~~~~~~
/open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp: In member function 'void fanya::parking::ParkingslotDetectMoudle::MsgCenterProc(hobot::dataflow::spMsgResourceProc, const MessageLists&)':
/open_explorer/parking/src/parkingslot_detection/parkingslot_detect_moudle.cpp:359:47: error: 'New' is not a member of 'hobot::communication::Subscriber<hobot::message::ProtoMsg<simple_proto::SimpleImage> >'
  359 |   auto subscriber = Subscriber<TestImageMsg>::New(
      |                                               ^~~
make[2]: *** [src/parkingslot_detection/CMakeFiles/psd.dir/build.make:76: src/parkingslot_detection/CMakeFiles/psd.dir/parkingslot_detect_moudle.cpp.o] Error 1
make[1]: *** [CMakeFiles/Makefile2:219: src/parkingslot_detection/CMakeFiles/psd.dir/all] Error 2
make[1]: *** Waiting for unfinished jobs....
[ 70%] Linking CXX shared library libaph.so
[ 70%] Built target aph
[ 80%] Linking CXX shared library libipm_trans.so
[ 80%] Built target ipm_trans
You pressed Ctrl+C!
make[2]: *** [src/location_map/CMakeFiles/loc.dir/build.make:76: src/location_map/CMakeFiles/loc.dir/location_map_module.cpp.o] Interrupt
ERROR: Exiting with code: 3
make[1]: *** [CMakeFiles/Makefile2:193: src/location_map/CMakeFiles/loc.dir/all] Interrupt
make: *** [Makefile:136: all] Interrupt
[conan]: conan install .. din/stable  -pr aarch64_gcc9.3 -o *:soc=j5 -o *:hbmem=True -o *:dataflow=True -o *:pagesize=64k -o *:asan_enable=False -s build_type=Release
build failed
