#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import argparse
import os
import shutil


# check and make build directory
def check_and_make_dir(dir_path):
    if os.path.exists(dir_path):
        shutil.rmtree(dir_path)
        os.makedirs(dir_path)
        assert os.path.exists(dir_path), dir_path
    else:
        os.makedirs(dir_path)
        assert os.path.exists(dir_path), dir_path


def run_command(conan_install_command):
    return_num = os.system(conan_install_command)
    if return_num != 0:
        exit(1)


def conan_build(conan_command):
    conan_install_command = "conan install .. din/stable {} ".format(
        conan_command)
    print("[conan]: {}".format(conan_install_command))
    run_command(conan_install_command)
    run_command("conan build ..")
    run_command("conan package  ..")


def cmake_build(cmake_command):
    cmake_compiler_command = \
        "cmake -DCONAN_ENABLE=OFF -DINDEPENDENT_COMPILATION=ON {} ..".format(
            cmake_command)
    print("[cmake]: {}".format(cmake_compiler_command))
    run_command(cmake_compiler_command)
    run_command("make -j4")
    run_command("make install")


def options_args(args, type):
    option = args.o
    dds_enable = 0
    cmake_or_conan_command = ""
    if option is not None:
        for opt in option:
            if opt == "hbmem=True":
                if type == "conan":
                    cmake_or_conan_command += " -o *:hbmem=True "
                else:
                    cmake_or_conan_command += " -DHBMEM_ENABLE=True "
            elif opt == "hbmem=False":
                if type == "conan":
                    cmake_or_conan_command += " -o *:hbmem=False "
                else:
                    cmake_or_conan_command += " -DHBMEM_ENABLE=False "
            elif opt == "dds=True":
                if type == "conan":
                    cmake_or_conan_command += " -o *:dds=True "
                    dds_enable = 0
                else:
                    cmake_or_conan_command += " -DDDS_ENABLE=True "
                    dds_enable = 1
            else:
                print(opt)
    else:
        print("options no")

    if type == "cmake":
        if dds_enable == 0:
            cmake_or_conan_command += " -DDDS_ENABLE=False "
        else:
            print("DDS_ENABLE=True")
    else:
        print("")

    return cmake_or_conan_command


def config_build(args):
    check_and_make_dir(args.build_path)
    os.chdir(args.build_path)

    # config支持conan和cmake两种方式：
    config = args.config
    tmp_config = config.split('_')
    if tmp_config[1] == "conan" or  tmp_config[2] == "conan":
        # conan: win64_conan, mac_conan, linux_conan, j3_conan, j5_conan,
        #        s32g_conan
        options = " "
        cmake_or_conan_command = options_args(args, "conan")
        if config == "win64_conan":
            options += "-pr win64"
        elif config == "mac_conan":
            options += "-pr mac"
        elif config == "linux_conan":
            options += "-pr linux"
        elif config == "j3_conan":
            options += "-pr aarch64_gcc6.5 -o *:soc=j3 -o *:hbmem=True"
        elif config == "j5_conan":
            options += "-pr aarch64_gcc9.3 -o *:soc=j5 -o *:hbmem=True"
        elif config == "s32g_conan":
            options += "-pr aarch64_fsl_gcc9.3 -o *:soc=s32g -o *:hbmem=True"
        elif config == "kbd_s32g_conan":
            options += "-pr aarch64_fsl_gcc10.2 -o *:soc=s32g -o *:hbmem=True"
        elif config == "x9u_conan":
            options += "-pr aarch64_sdrv_gcc8.2 -o *:soc=x9u"
        elif config == "android_conan":
            options += "-pr android_r24"
        else:
            print("config: {} is not supported.".format(config))
            exit(1)
        options += cmake_or_conan_command
        parse_build_type(args, options, "conan")
    else:
        # cmake: win64_cmake, mac_cmake, linux_cmake, j3_cmake, j5_cmake,
        #        s32g_cmake
        cmake_or_conan_command = options_args(args, "cmake")
        cmake_command = " "
        if config == "win64_cmake":
            print("CMake unsupported complier platform.")
        elif config == "mac_cmake":
            print("CMake unsupported complier platform.")
        elif config == "linux_cmake":
            cmake_command = "-DSOC=x86_64 -DENABLE_PERFETTO=True"
        elif config == "j3_cmake":
            cmake_command = "-DSOC=j3 -DAARCH_J3=True -DHBMEM_ENABLE=True" + \
                " -DENABLE_PERFETTO=True "
        elif config == "j5_cmake":
            cmake_command = "-DSOC=j5 -DAARCH_J5=True -DHBMEM_ENABLE=True" + \
                " -DENABLE_PERFETTO=True "
        elif config == "s32g_cmake":
            cmake_command = "-DSOC=s32g -DS32G=True -DHBMEM_ENABLE=True" + \
                " -DENABLE_PERFETTO=True"
        elif config == "kbd_s32g_cmake":
            cmake_command = "-DSOC=s32g -DS32G=True -DHBMEM_ENABLE=True" + \
                " -DENABLE_PERFETTO=True"
        elif config == "x9u_cmake":
            cmake_command = "-DSOC=x9u -DX9U=True -DENABLE_PERFETTO=True"
        else:
            print("config: {} is not supported.".format(config))
            exit(1)
        cmake_command += cmake_or_conan_command
        parse_build_type(args, cmake_command, "cmake")


def build_with_conan(args):
    check_and_make_dir(args.build_path)
    os.chdir(args.build_path)
    options = " "

    # os: [Windows, Macos, Linux]
    operate_system = args.os
    if operate_system == "Windows":
        options += "-pr win64"
    elif operate_system == "Macos":
        options += "-pr mac"
    elif operate_system == "Linux":
        # soc: [x86_64, j3, j5, s32g]
        soc = args.soc
        toolchains = args.pr
        if toolchains is None and soc == "x86_64":
            options += "-pr linux"
        # toolchains [aarch64_gcc6.5] corresponds to soc [j3]
        elif toolchains == "aarch64_gcc6.5" and soc == "j3":
            options += "-pr aarch64_gcc6.5 -o *:soc=j3 -o *:hbmem=True"
        # toolchains [aarch64_gcc9.3] corresponds to soc [j5]
        elif toolchains == "aarch64_gcc9.3" and soc == "j5":
            options += "-pr aarch64_gcc9.3 -o *:soc=j5 -o *:hbmem=True"
        # soc: toolchains [aarch64_fsl_gcc9.3] corresponds to soc [s32g]
        elif toolchains == "aarch64_fsl_gcc9.3" and soc == "s32g":
            options += "-pr aarch64_fsl_gcc9.3 -o *:soc=s32g -o *:hbmem=True"
        # soc: toolchains [aarch64_fsl_gcc10.2] corresponds to soc [kbd_s32g]
        elif toolchains == "aarch64_fsl_gcc10.2" and soc == "s32g":
            options += "-pr aarch64_fsl_gcc10.2 -o *:soc=s32g -o *:hbmem=True"
        # soc: toolchains [aarch64_sdrv_gcc8.2] corresponds to soc [x9u]
        elif toolchains == "aarch64_sdrv_gcc8.2" and soc == "x9u":
            options += "-pr aarch64_sdrv_gcc8.2 -o *:soc=x9u "
        else:
            print("toolchains: {} & soc: {} is not incompatible.".format(
                toolchains, soc))
            exit(1)
    elif operate_system == "Android":
        toolchains = args.pr
        # soc: toolchains [android_r24]
        if toolchains == "android_r24":
            options += "-pr android_r24"

    else:
        print("operate system: {} is not supported.".format(operate_system))
        exit(1)

    parse_build_type(args, options, "conan")


def build_with_cmake(args):
    check_and_make_dir(args.build_path)
    os.chdir(args.build_path)
    cmake_command = " "
    cmake_or_conan_command = options_args(args, "cmake")
    # os: [Windows, Macos, Linux]
    operate_system = args.os
    if operate_system == "Windows":
        print("CMake unsupported complier platform: {}".format(operate_system))
    elif operate_system == "Macos":
        print("CMake unsupported complier platform: {}".format(operate_system))
    elif operate_system == "Linux":
        # soc: [x86_64, j3, j5, s32g]
        soc = args.soc
        toolchains = args.pr
        if toolchains is None and soc == "x86_64":
            cmake_command = "-DSOC=x86_64 -DENABLE_PERFETTO=True"
        # toolchains [aarch64_gcc6.5] corresponds to soc [j3]
        elif toolchains == "aarch64_gcc6.5" and soc == "j3":
            cmake_command = "-DSOC=j3 -DAARCH_J3=True -DHBMEM_ENABLE=True" + \
                " -DENABLE_PERFETTO=True"
        # toolchains [aarch64_gcc9.3] corresponds to soc [j5]
        elif toolchains == "aarch64_gcc9.3" and soc == "j5":
            cmake_command = "-DSOC=j5 -DAARCH_J5=True -DHBMEM_ENABLE=True" + \
                " -DENABLE_PERFETTO=True "
        # soc: toolchains [aarch64_fsl_gcc9.3] corresponds to soc [s32g]
        elif toolchains == "aarch64_fsl_gcc9.3" and soc == "s32g":
            cmake_command = "-DSOC=s32g -DS32G=True -DHBMEM_ENABLE=True" + \
                "-DENABLE_PERFETTO=True  "
        # soc: toolchains [aarch64_fsl_gcc10.2] corresponds to soc [kbd_s32g]
        elif toolchains == "aarch64_fsl_gcc10.2" and soc == "s32g":
            cmake_command = "-DSOC=s32g -DS32G=True -DHBMEM_ENABLE=True" + \
                "-DENABLE_PERFETTO=True  "
        # soc: toolchains [aarch64_sdrv_gcc8.2] corresponds to soc [x9u]
        elif toolchains == "aarch64_sdrv_gcc8.2" and soc == "x9u":
            cmake_command = "-DSOC=x9u -DX9U=True -DENABLE_PERFETTO=True"
        else:
            print("toolchains: {} & soc: {} is not incompatible.".format(
                toolchains, soc))
            exit(1)
    else:
        print("operate system: {} is not supported.".format(operate_system))
        exit(1)
    cmake_command += cmake_or_conan_command
    parse_build_type(args, cmake_command, "cmake")


def parse_build_type(args, command, type):
    build_type = args.build_type
    if type == "conan":
        if build_type == "Release":
            command += " -s build_type=Release"
        elif build_type == "Debug":
            command += " -s build_type=Debug"
        else:
            print("build_type: {} is not supported".format(build_type))
            exit(1)

        conan_build(command)
    elif type == "cmake":
        if build_type == "Release":
            command += " -DCMAKE_BUILD_TYPE:STRING=Release"
        elif build_type == "Debug":
            command += " -DCMAKE_BUILD_TYPE:STRING=Debug"
        else:
            print("build_type: {} is not supported".format(build_type))
            exit(1)

        cmake_build(command)
    else:
        print("type: {} is not supported.".format(type))


def parse_argparse():
    description = "Build script for TROS.A schedulegroup example"
    parser = argparse.ArgumentParser(description=description)
    build_path = os.path.join(os.getcwd(), "build")
    parser.add_argument('--enable_conan', action='store_true')
    parser.add_argument('--config',
                        choices=['win64_conan', 'mac_conan', 'linux_conan',
                                 'j3_conan', 'j5_conan', 's32g_conan', 'kbd_s32g_conan',
                                 'win64_cmake', 'mac_cmake', 'linux_cmake',
                                 'j3_cmake', 'j5_cmake', 's32g_cmake',
                                 'android_conan', 'x9u_conan', 'x9u_cmake'])
    parser.add_argument('--pr',
                        choices=['aarch64_gcc6.5', 'aarch64_gcc9.3',
                                 'aarch64_fsl_gcc9.3', 'aarch64_fsl_gcc10.2','android_r24',
                                 'aarch64_sdrv_gcc8.2'])
    parser.add_argument('--os',
                        choices=['Windows', 'Macos', 'Linux', 'Android'])
    parser.add_argument('--o',
                        choices=['dds=True', 'hbmem=True', 'hbmem=False'],
                        nargs='+')
    parser.add_argument('--soc',
                        choices=['x86_64', 'j3', 'j5', 's32g', 'x9u'])
    parser.add_argument('--build_type',
                        choices=['Release', 'Debug'],
                        default='Release')
    parser.add_argument('--build_path',
                        help='build directory\'s path, default is <pwd>/build',
                        default=build_path)
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_argparse()
    if args.config:
        config_build(args)
    else:
        if args.enable_conan:
            build_with_conan(args)
        else:
            build_with_cmake(args)
