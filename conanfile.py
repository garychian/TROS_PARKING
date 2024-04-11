from conans import ConanFile, CMake, tools
import re
import os


class PackageConanFile(ConanFile):
    name = "fanya_parking"
    author = "developer<developer@develop.com>"
    default_channel = "stable"
    default_user = "tros"
    settings = "os", "compiler", "build_type", "arch"

    requires = ["dataflow/[x.x.x]@bole/stable",
                ("hobot-bole-protocol/1.4.33@bole/stable", "private")]
    tool_requires = ["gtest/1.8.0@thirdparty/stable"]

    version_major = None
    version_minor = None
    version_patch = None
    version_revision = None

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

    generators = "cmake_find_package"
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
