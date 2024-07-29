from conans import ConanFile, CMake, tools
import re
import os


class ScheduleGroupConan(ConanFile):
    name = "schedulegroup"
    settings = "os", "compiler", "build_type", "arch"
    options = {}
    requires = ["hlog/[1.4.x]@bole/stable",
                "nlohmann_json/3.7.3@thirdparty/stable"]
    tool_requires = ["gtest/1.8.0@thirdparty/stable"]
    generators = "cmake_find_package"
    exports_sources = ["example/*"]

    _cmake = None

    version_major = None
    version_minor = None
    version_patch = None

    def set_version(self):
        content = tools.load(os.path.join(self.recipe_folder, "version.txt"))
        self.version = re.search(r"version\s*=\s*(\d+\.(?:\d+\.)*\d+)",
                                 content).group(1)
        version_list = self.version.split(".")
        self.version_major = version_list[0]
        self.version_minor = version_list[1]
        self.version_patch = version_list[2]

    def imports(self):
        if self.settings.os == "Windows":
            install_dir = "build_subfolder/bin/{}".format(
                self.settings.build_type)
        else:
            install_dir = "build_subfolder/bin"
        self.copy("*.so*", install_dir, "lib")
        self.copy("*", install_dir, "bin")
        self.copy("*.dll*", install_dir, "lib")
        self.copy("*.dylib*", install_dir, "lib")
        self.copy("*", install_dir, "etc")

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def source(self):
        git = tools.Git(folder=self._source_subfolder)
        git.clone("git@gitlab.hobot.cc:ptd/ap/middleware/schedulegroup.git",
                  "develop")

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        if 'distro' in self.settings.compiler[self.settings.compiler.value] \
                .fields:
            if self.settings.os == "Linux" \
                    and self.settings.compiler.distro == "fsl":
                self._cmake.definitions["S32G"] = True
        else:
            print("distro none")
        self._cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type
        self._cmake.definitions["ARM"] = self.settings.arch == "armv8"
        # set version to cmake
        self._cmake.definitions["VERSION_MAJOR"] = self.version_major
        self._cmake.definitions["VERSION_MINOR"] = self.version_minor
        self._cmake.definitions["VERSION_PATCH"] = self.version_patch
        self._cmake.configure(build_folder=self._build_subfolder)
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        cmake.install()

    def package(self):
        cmake = self._configure_cmake()
        # cmake.install()
        self.copy("*",
                  src=os.path.join(self.source_folder, "output",
                                   self.name, "include"),
                  dst="include",
                  keep_path=True, symlinks=True)
        self.copy("*",
                  src=os.path.join(self.source_folder, "output",
                                   self.name, "lib"),
                  dst="lib",
                  keep_path=True, symlinks=True)
        self.copy("version.txt",
                  src=self.source_folder,
                  dst=os.path.join(self.source_folder, "example"))
        self.copy("schedulegroup.cmake",
                  src=os.path.join(self.source_folder, "cmake"),
                  dst=os.path.join(self.source_folder, "example/cmake"))

    def package_info(self):
        self.cpp_info.libs = [self.name]
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", self.name)
