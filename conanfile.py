from conans import ConanFile, CMake, tools
import re
import os


class PackageConanFile(ConanFile):
    name = "fanya_parking"
    author = "developer<developer@develop.com>"
    default_channel = "stable"
    default_user = "tros"
    settings = "os", "compiler", "build_type", "arch"
    generators = ["cmake_find_package", "json"]

    requires = ["dataflow/[x.x.x]@bole/stable",
                ("hobot-bole-protocol/1.4.33@bole/stable", "private")]
    tool_requires = ["gtest/1.8.0@thirdparty/stable"]

    version_major = None
    version_minor = None
    version_patch = None
    version_revision = None

    requires = [
        # direct deps
        ("opencv/3.4.5@thirdparty/stable"),
        ("nlohmann_json/3.7.3@thirdparty/stable"),
        ("hobot-bole-protocol/1.4.43@bole/stable"),
        ("easy_dnn/1.10.6@bole/stable", "override"),
        ("sensors/1.10.5@bole/stable"),
        ("sm_client/1.6.3@tros/stable", "override"),
        ("common/1.6.3@tros/stable", "override"),
        ("hlog/1.10.2@bole/stable", "override"),
        ("communication/1.10.5@bole/stable", "override"),
        ("message/[1.7.x]@bole/stable", "override"),
        ("dataflow/1.10.5@bole/stable", "override"),
        ("convert_assistant/2.5.3@thirdparty/stable"),
        ("fanya-protocol/1.0.0@ltc/fanya")
    ]

    def set_version(self):
        content = tools.load(os.path.join(self.recipe_folder, "version.txt"))
        self.version = re.search(r"version\s*=\s*(\d+\.(?:\d+\.)*\d+)",
                                 content).group(1)
        version_list = self.version.split(".")
        self.version_major = version_list[0]
        self.version_minor = version_list[1]
        self.version_patch = version_list[2]
        if len(version_list) == 4:
            self.version_revision = version_list[3]

    def requirements(self):
        """add requirements"""

    def imports(self):
        """imports function"""
        targets = []
        targets.append("parking")
        for target in targets:
                # tros_common_lib
            self.copy("libdataflow*.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libcommunication*.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libplugin-zmq*.so", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libsensor_center-share*.so*",
                          "package/%s/tros_common_lib" % target, "lib")
            self.copy("libsensor_center_common*.so*",
                          "package/%s/tros_common_lib" % target, "lib")
            self.copy("libcamera*.so*", "package/%s/tros_common_lib" % target,
                          "lib/sensor_plugin")

            self.copy("libhobotlog*.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libhlog*.so*", "package/%s/tros_common_lib" % target, "lib")
            self.copy("libeasy_dnn*.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libdnn*.so*", "package/%s/tros_common_lib" % target, "lib")
            self.copy("libprotobuf*.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libvehicleio_service*.so*",
                          "package/%s/tros_common_lib" % target, "lib")
            self.copy("libopencv_world.so.3.4", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libschedulegroup*.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libtime*.so*", "package/%s/tros_common_lib" % target, "lib")
            self.copy("libconvert_assistant.so*",
                          "package/%s/tros_common_lib" % target, "lib")
            self.copy("libapa_postprocessor.so*",
                          "package/%s/tros_common_lib" % target, "lib")
            self.copy("libperfetto_sdk.so", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libsm_client.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libcommonipc.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libsm_client.so*", "package/%s/tros_common_lib" % target,
                          "lib")
            self.copy("libsm_client.so*", "package/%s/tros_common_lib" % target,
                          "lib")


            # if self.options.soc == "j3":
            #     self.copy(
            #             "libhbrt_bernoulli_aarch64*.so*",
            #             "package/%s/tros_common_lib" % target,
            #             "lib",
            #         )
            # elif self.options.soc == "j5":
            self.copy("libhbrt_bayes_aarch64*.so*",
                              "package/%s/tros_common_lib" % target, "lib")

            # if self.options.asan_enable == "True":
            #     self.copy("libasan.so*", "package/%s/tros_common_lib" % target, "lib")

            # bins
            self.copy("mainboard2*", "package/%s" % target, "bin")
            self.copy("launch2*", "package/%s" % target, "bin")

            # tools
            self.copy("dataflow_tool*", "package/%s/tools" % target, "bin")
            self.copy("communication_tool*", "package/%s/tools" % target,
                          "bin")
            
        # self.copy("communication_tool*", "package/%s/tools" % target,
        #                   "bin")

    # generators = "cmake_find_package"
    _cmake = None

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        settings = self.settings
        self._cmake = CMake(self)
        if 'distro' in settings.compiler[self.settings.compiler.value].fields:
            if settings.os == "Linux" and settings.compiler.distro == "fsl":
                self._cmake.definitions["S32G"] = True
        self._cmake.definitions["OS"] = self.settings.os
        self._cmake.definitions["ARCH"] = self.settings.arch
        self._cmake.definitions["build_type"] = self.settings.build_type
        self._cmake.definitions["ARM"] = self.settings.arch == "armv8"
        self._cmake.definitions["VERSION_MAJOR"] = self.version_major
        self._cmake.definitions["VERSION_MINOR"] = self.version_minor
        self._cmake.definitions["VERSION_PATCH"] = self.version_patch
        self._cmake.configure(build_folder=self._build_subfolder)
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.configure()
        cmake.build()
        cmake.install()

    def package(self):
        source_base = os.path.join(self.build_folder, "output", self.name)
        self.copy("*", src=os.path.join(source_base, "bin"), dst="bin",
                  keep_path=True, symlinks=True)
        self.copy("*", src=os.path.join(source_base, "include"), dst="include",
                  keep_path=True, symlinks=True)
        self.copy("*", src=os.path.join(source_base, "lib"), dst="lib",
                  keep_path=True, symlinks=True)
        self.copy("*", src=os.path.join(source_base, "config"), dst="config",
                  keep_path=True, symlinks=True)

    def package_info(self):
        self.cpp_info.libs = [self.name]
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", self.name)
