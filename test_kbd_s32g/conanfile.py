from conans import ConanFile, CMake
import os


class communication_exampleConan(ConanFile):
    name = "communication_example"
    version = "1.10.5"
    settings = "os", "compiler", "build_type", "arch"
    options = {"soc": ["j3", "j5", "s32g", "x9u", "ignore"],
               "hbmem": [False, True],
               "dds": [False, True],
               "build_shm": [False, True],
               "benchmark": [None, "fastdds"]}
    default_options = {"soc": "ignore",
                       "hbmem": False,
                       "dds": False,
                       "build_shm": False,
                       "benchmark": None}

    use_perfetto = True

    requires = [("communication/1.13.5@bole/stable"),
                ("fanya-protocol/1.0.0@ltc/fanya"),
                ("hobot-bole-protocol/1.7.64.1@bole/stable")]

    generators = "cmake_find_package"
    _cmake = None

    def imports(self):
        if self.settings.os == "Windows":
            install_dir = "build_subfolder/bin/{}".format(
                self.settings.build_type)
            self.copy("*hlog*.dll", install_dir, "bin")
        else:
            install_dir = "build_subfolder/bin"
        self.copy("*communication*", install_dir, "lib")
        self.copy("*plugin*", install_dir, "lib")
        self.copy("*hlog.*", install_dir, "lib")
        self.copy("*hlogd.*", install_dir, "lib")
        self.copy("*schedulegroup*", install_dir, "lib")
        self.copy("*perfetto_sdk.*", install_dir, "lib")
        self.copy("*fanya*", install_dir, "lib")

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)

        self._cmake.definitions["AARCH_J3"] = self.options.soc == "j3"
        if self.options.soc == "j5":
            self._cmake.definitions["AARCH_J5"] = True
        if self.options.soc == "s32g":
            self._cmake.definitions["S32G"] = True
        if self.options.soc == "x9u":
            self._cmake.definitions["X9U"] = True

        if self.options.benchmark == "fastdds":
            self._cmake.definitions["BENCHMARK_FASTDDS"] = True

        self._cmake.definitions["HBMEM_ENABLE"] = self.options.hbmem
        self._cmake.definitions["DDS_ENABLE"] = self.options.dds
        self._cmake.definitions["BUILD_SHM"] = self.options.build_shm
        self._cmake.definitions["INDEPENDENT_COMPILATION"] = True
        self._cmake.definitions["DEPS_GLOG"] = False
        self._cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type
        # self._cmake.definitions["SDIO_SUPPORT"] =
        # self.settings.arch == "armv8"
        if self.settings.os == "Android":
            self._cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = os.environ['CONAN_CMAKE_TOOLCHAIN_FILE']
            self._cmake.definitions["ANDROID_NATIVE_API_LEVEL"] = os.environ['ANDROID_NATIVE_API_LEVEL']
            self._cmake.definitions["ANDROID_ABI"] = os.environ['ANDROID_ABI']

        self._cmake.definitions["ENABLE_PERFETTO"] = False
        if self.settings.os == "Linux":
            if self.settings.arch == "x86_64" and self.options.hbmem == True:
                self._cmake.definitions["AARCH_x86_HBMEM_ENABLE"] = True
            if self.use_perfetto:
                if self.options.soc == "j3" and self.options.hbmem == False:
                    print("[example][conan] no perfetto on J3 with hbmen, ENABLE_PERFETTO False")
                    self._cmake.definitions["ENABLE_PERFETTO"] = False
                else:
                    print("[example][conan] ENABLE_PERFETTO True")
                    self._cmake.definitions["ENABLE_PERFETTO"] = True
            else:
                print("[example][conan] ENABLE_PERFETTO False")
                self._cmake.definitions["ENABLE_PERFETTO"] = False

        self._cmake.configure(build_folder=self._build_subfolder)
        return self._cmake

    # merge static lib

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):

        self.cpp_info.libs = [self.name]
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", self.name)
        self.cpp_info.names["cmake_find_package"] = self.name
