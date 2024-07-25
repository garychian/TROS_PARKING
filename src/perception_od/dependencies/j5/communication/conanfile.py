from conans import ConanFile, CMake, tools
import os
from string import Template


class communicationConan(ConanFile):
    name = "communication"
    version = "1.4.5"
    settings = "os", "compiler", "build_type", "arch"
    options = {"soc": ["j3", "j5", "s32g", "ignore"],
               "hbmem": [False, True],
               "dds": [False, True],
               "build_shm": [False, True],
               "benchmark": [None, "fastdds"]}
    default_options = {"soc": "ignore",
                       "hbmem": False,
                       "dds": False,
                       "build_shm": False,
                       "benchmark": None}

    requires = [("protobuf/3.19.3@thirdparty/stable"),
                ("zeromq/4.3.4@thirdparty/stable"),
                ("message/[1.4.x]@bole/stable"),
                ("schedulegroup/[1.4.x]@bole/stable")]
    exports_sources = ["example/*", "example_bolemsg/*", "config/*"]
    tool_requires = ["gtest/1.8.0@thirdparty/stable"]

    def requirements(self):
        if self.options.soc == "j3":
            self.requires("j3dvb_system/2022.11.28@bsp/stable")
        elif self.options.soc == "j5":
            self.requires("j5dvb_system/2022.08.10@bsp/stable")
        elif self.options.soc == "s32g":
            self.requires("j5dvb_system/2022.08.10@bsp/stable")

        if self.options.dds:
            self.requires("fast-dds/2.1.1@thirdparty/stable")
            self.requires("cyclonedds/0.10.0@thirdparty/stable")
        if self.settings.os == "Android":
            self.requires("libsodium/1.0.18@thirdparty/stable")

    generators = "cmake_find_package"
    _cmake = None

    def imports(self):
        self.copy("libfmt.a", "mergelib", "lib")
        self.copy("libprotobuf.a", "mergelib", "lib")
        self.copy("*hlog.*", "build_subfolder/bin", "lib")
        self.copy("*hlogd.*", "build_subfolder/bin", "lib")
        self.copy("*hlog.*", "tool_dependency_library", "lib")
        self.copy("*schedulegroup*", "build_subfolder/bin", "lib")
        self.copy("*schedulegroup*", "tool_dependency_library", "lib")
        if self.settings.os == "Windows":
            install_dir = "build_subfolder/test/interface/{}".format(
                self.settings.build_type)
            self.copy("*schedulegroup*", install_dir, "lib")
            self.copy("*hlog*", install_dir, "lib")
            self.copy("*gtest*.dll*", install_dir, "bin")
            self.copy("libzmq-v140-mt-4_3_4.dll", install_dir, "bin")
            self.copy("*gtest*.dll*", "build_subfolder/bin", "bin")
            self.copy("libzmq-v140-mt-4_3_4.dll", "build_subfolder/bin", "bin")

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def source(self):
        git = tools.Git(folder=self._source_subfolder)
        git.clone(
            "git@gitlab.hobot.cc:ptd/ap/middleware/communication.git", "test-conan")

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)

        self._cmake.definitions["AARCH_J3"] = self.options.soc == "j3"
        if self.options.soc == "j5" or self.options.soc == "s32g":
            self._cmake.definitions["AARCH_J5"] = True
        if self.options.soc == "s32g":
            self._cmake.definitions["S32G"] = True

        if self.options.benchmark == "fastdds":
            self._cmake.definitions["BENCHMARK_FASTDDS"] = True

        self._cmake.definitions["HBMEM_ENABLE"] = self.options.hbmem
        self._cmake.definitions["DDS_ENABLE"] = self.options.dds
        self._cmake.definitions["BUILD_SHM"] = self.options.build_shm

        self._cmake.definitions["DEPS_GLOG"] = False
        self._cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type
        # self._cmake.definitions["SDIO_SUPPORT"] =
        # self.settings.arch == "armv8"

        self._cmake.configure(build_folder=self._build_subfolder)
        return self._cmake

    # merge static lib

    def _mergelib(self, build_dir):
        if self.settings.os == "Windows":
            print("[conan] [communication] Windows does not need merge")
        else:
            if self.settings.build_type == "Debug":
                lib_dir = os.path.join(
                    self.source_folder,
                    'output/communication/lib/libcommunication-staticd.a')
            else:
                lib_dir = os.path.join(
                    self.source_folder, 'output/communication/lib/libcommunication-static.a')
            print(
                "[conan] [communication] staic_dir = {}".format(lib_dir))
            mergelib_dir = os.path.join(build_dir, 'mergelib')
            print("[conan] [communication] mergelib_dir = {}".format(mergelib_dir))
            aarch_6_hbipc_dir = os.path.join(
                self.source_folder, 'thirdparty/lib/linux_aarch64_gcc6.5.0')
            aarch_9_hbipc_dir = os.path.join(
                self.source_folder, 'thirdparty/lib/linux_aarch64_gcc9.3.0')
            linux_hbipc_dir = os.path.join(
                self.source_folder, 'thirdparty/lib/linux_x86_gcc5.4.0')
            ArCommLib = "ar rc {} *.o".format(lib_dir)
            os.chdir(mergelib_dir)
            os.system("ar x  libfmt.a")
            os.system("ar x  libprotobuf.a")
            os.system(ArCommLib)
            os.system("rm *.o")

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        build_dir = os.getcwd()
        print("[conan] [communication] build_dir = {}".format(build_dir))
        cmake.install()
        self._mergelib(build_dir)
        if self.settings.os == "Windows":
            self.copy("*.dll",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
            self.copy("communication.*",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
            self.copy("communicationd.*",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
            self.copy("boleidlc-plugin-api*",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
            self.copy("plugin-zmq*",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
            self.copy("*PyComm*",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
        elif self.settings.os == "Macos":
            self.copy("*.dylib",
                      src=os.path.join(self.source_folder, "output",
                                       self.name, "lib"),
                      dst="lib",
                      keep_path=True, symlinks=True)
        self.copy("*.so*",
                  src=os.path.join(self.source_folder, "output",
                                   self.name, "lib"),
                  dst="lib",
                  keep_path=True, symlinks=True)
        self.copy("*",
                  src=os.path.join(self.source_folder, "output",
                                   self.name, "include"),
                  dst="include",
                  keep_path=True, symlinks=True)
        self.copy("*",
                  src=os.path.join(self.source_folder, "output",
                                   self.name, "bin"),
                  dst="bin",
                  keep_path=True, symlinks=True)
        self.copy("*",
                  src=os.path.join(self.source_folder, "output",
                                   self.name, "config"),
                  dst="config",
                  keep_path=True, symlinks=True)

    def package_info(self):

        self.cpp_info.libs = [self.name]
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", self.name)
        self.cpp_info.names["cmake_find_package"] = self.name
