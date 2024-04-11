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


def make_sym_links():
    targets = [
        "libplugin-zmq{}.so", "libapa{}.so", "libsensor_center-share{}.so",
        "libcamera{}.so", "libhlog{}.so.1", "libeasy_dnn{}.so"
    ]

    base = os.path.join(os.path.realpath(os.curdir), "..", "output", "apa",
                        "libs")
    for target in targets:

        os.chdir(base)
        src = target.format("d")
        dst = target.format("")
        os.symlink(src, dst)


def run_command(conan_install_command):
    return_num = os.system(conan_install_command)
    if return_num != 0:
        exit(1)


def conan_build(conan_command):
    conan_install_command = "conan install .. din/stable {} -u".format(
        conan_command)
    print("[conan]: {}".format(conan_install_command))
    run_command(conan_install_command)
    run_command("mkdir -p ../output/")
    run_command("rm -rf ../output/*")
    
    run_command("conan build ..")
    run_command("conan package ..")
    run_command("cp -r -p package/parking ../output/")
    # run_command("cp -rf ../3rd_party/* ../output/apa/libs/")


def cmake_build(cmake_command):
    run_command("pwd")
    cmake_compiler_command = \
        "cmake -DCONAN_ENABLE=OFF -DINDEPENDENT_COMPILATION=ON {} ..".format(
            cmake_command)
    print("[cmake]: {}".format(cmake_compiler_command))
    run_command(cmake_compiler_command)
    run_command("mkdir -p ../output/")
    run_command("rm -rf ../output/*")
    run_command("make -j4")
    run_command("make install")


def config_build(args):
    check_and_make_dir(args.build_path)
    os.chdir(args.build_path)

    # config支持conan和cmake两种方式：
    config = args.config
    tmp_config = config.split('_')
    if tmp_config[1] == "conan":
        # conan: win64_conan, mac_conan, linux_conan, j3_conan, j5_conan,
        #        s32g_conan
        options = " "
        if config == "win64_conan":
            options += "-pr win64"
        elif config == "mac_conan":
            options += "-pr mac"
        elif config == "linux_conan":
            options += "-pr linux"
        elif config == "j3_conan":
            options += "-pr aarch64_gcc6.5 -o *:soc=j3 -o *:hbmem=True -o *:dataflow=True"
        elif config == "j5_conan":
            options += "-pr aarch64_gcc9.3 -o *:soc=j5 -o *:hbmem=True -o *:dataflow=True"
        elif config == "s32g_conan":
            options += "-pr aarch64_fsl_gcc9.3 -o *:soc=s32g -o *:hbmem=True -o *:dataflow=True"
        else:
            print("config: {} is not supported.".format(config))
            exit(1)

        if args.asan_enable:
            options += " -o *:asan_enable=True"
        else:
            options += " -o *:asan_enable=False"
        parse_build_type(args, options, "conan")
    else:
        # cmake: win64_cmake, mac_cmake, linux_cmake, j3_cmake, j5_cmake,
        #        s32g_cmake
        cmake_command = " "
        if config == "win64_cmake":
            print("CMake unsupported complier platform.")
        elif config == "mac_cmake":
            print("CMake unsupported complier platform.")
        elif config == "linux_cmake":
            cmake_command = "-DSOC=x86_64 -Dsoc= -Ddataflow=True -DAARCH64=False"
        elif config == "j3_cmake":
            cmake_command = "-DSOC=j3 -Dsoc=j3 -Ddataflow=True -DAARCH64=True"
        elif config == "j5_cmake":
            cmake_command = "-DSOC=j5 -Dsoc=j5 -Ddataflow=True -DAARCH64=True -Dgccversion=9.3"
        elif config == "s32g_cmake":
            print("CMake unsupported complier platform.")
        else:
            print("config: {} is not supported.".format(config))
            exit(1)

        parse_build_type(args, cmake_command, "cmake")

    if args.build_type == "Debug":
        make_sym_links()


def build_with_conan(args):
    check_and_make_dir(args.build_path)
    os.chdir(args.build_path)
    options = " "

    if args.asan_enable:
        options += "-o *:asan_enable=True"
    else:
        options += "-o *:asan_enable=False"
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
            options += "-pr aarch64_gcc6.5 -o *:soc=j3 -o *:hbmem=True -o *:dataflow=True"
        # toolchains [aarch64_gcc9.3] corresponds to soc [j5]
        elif toolchains == "aarch64_gcc9.3" and soc == "j5":
            options += "-pr aarch64_gcc9.3 -o *:soc=j5 -o *:hbmem=True -o *:dataflow=True"
        # soc: toolchains [aarch64_fsl_gcc9.3] corresponds to soc [s32g]
        elif toolchains == "aarch64_fsl_gcc9.3" and soc == "s32g":
            options += "-pr aarch64_fsl_gcc9.3 -o *:soc=s32g -o *:hbmem=True -o *:dataflow=True"
        else:
            print("toolchains: {} & soc: {} is not incompatible.".format(
                toolchains, soc))
            exit(1)
    else:
        print("operate system: {} is not supported.".format(operate_system))
        exit(1)

    parse_build_type(args, options, "conan")


def build_with_cmake(args):
    check_and_make_dir(args.build_path)
    os.chdir(args.build_path)
    cmake_command = " "

    if args.asan_enable:
        options += "-o *:asan_enable=True"
    else:
        options += "-o *:asan_enable=False"
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
            cmake_command = "-DSOC=x86_64 -Dsoc= -DAARCH64="
        # toolchains [aarch64_gcc6.5] corresponds to soc [j3]
        elif toolchains == "aarch64_gcc6.5" and soc == "j3":
            cmake_command = "-DSOC=j3 -Dsoc=j3 -Ddataflow=True -DAARCH64=True"
        # toolchains [aarch64_gcc9.3] corresponds to soc [j5]
        elif toolchains == "aarch64_gcc9.3" and soc == "j5":
            cmake_command = "-DSOC=j5 -Dsoc=j5 -Ddataflow=True -DAARCH64=True"
        # soc: toolchains [aarch64_fsl_gcc9.3] corresponds to soc [s32g]
        elif toolchains == "aarch64_fsl_gcc9.3" and soc == "s32g":
            print("CMake unsupported complier platform: {}".format(
                operate_system))
        else:
            print("toolchains: {} & soc: {} is not incompatible.".format(
                toolchains, soc))
            exit(1)
    else:
        print("operate system: {} is not supported.".format(operate_system))
        exit(1)

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
    build_path = os.path.join(os.path.dirname(__file__), "../build")
    parser.add_argument('--enable_conan', action='store_true')
    parser.add_argument('--config',
                        '-c',
                        choices=[
                            'win64_conan', 'mac_conan', 'linux_conan',
                            'j3_conan', 'j5_conan', 's32g_conan',
                            'win64_cmake', 'mac_cmake', 'linux_cmake',
                            'j3_cmake', 'j5_cmake', 's32g_cmake'
                        ])
    parser.add_argument(
        '--pr',
        choices=['aarch64_gcc6.5', 'aarch64_gcc9.3', 'aarch64_fsl_gcc9.3'])
    parser.add_argument('--os', choices=['Windows', 'Macos', 'Linux'])
    parser.add_argument('--asan_enable',
                        '-a',
                        action='store_true',
                        help='build with asan')
    parser.add_argument('--soc', choices=['x86_64', 'j3', 'j5', 's32g'])
    parser.add_argument('--build_type',
                        '-b',
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
