# -*- coding: utf-8 -*-
import pathlib
import re
import time
import os
import io
import sys
import shutil
from shutil import copy
from string import Template
from functools import wraps

# First parameter = platform [linux | win64 | mac | aarch64_gcc9.3 | aarch64_gcc6.5]
# Second parameter = option_soc [*:soc=j3 |*:pcie=True | *:hbmem=False]
 
def config_decorator(func):
    @wraps(func)
    def inner(*args, **kwargs):
        file_path = pathlib.Path(__file__).resolve().parent.parent.joinpath("CMakeLists.txt")
        print(f"pathlib.Path(__file__):{pathlib.Path(__file__)}")
        backup_path = file_path.parent.joinpath("CMakeLists_backup.txt")
        if backup_path.exists():
            backup_path.unlink()
        with open(file_path, mode="r", encoding="utf-8") as r_f, open(
            backup_path, "w", encoding="utf-8"
        ) as w_f:
            for line in r_f:
                if not re.findall(r"project(.*)", line.strip()):
                    w_f.write(line)
                else:
                    append_data = "set(COVERAGE ON)\n"
                    w_f.write(line + append_data)
        print(f"file_path:{file_path}")
        print(f"backup_path:{backup_path}")
        backup_file_path = file_path.parent.joinpath("CMakeLists.txt.bak")
        file_path.rename(backup_file_path)
        backup_path.rename(backup_path.parent.joinpath("CMakeLists.txt"))
        func(*args, **kwargs)
        backup_file_path.replace(file_path.parent.joinpath("CMakeLists.txt"))

    return inner

@config_decorator
def compile():
    print("compile start")
    build_start()
    print("compile end")

# make dir
def check_and_make_dir(dir_path):
    if os.path.exists(dir_path):
        shutil.rmtree(dir_path)
        os.makedirs(dir_path)
        assert os.path.exists(dir_path), dir_path
    else:
        os.makedirs(dir_path)
        assert os.path.exists(dir_path), dir_path

def conan_install_argument(platform):
    print("[conan] Now platform:{}".format(platform))
    if len(sys.argv) == 3 and sys.argv[2] != "coverage":
        conan_install = "-o {} -pr {}".format(sys.argv[2], platform)
    elif len(sys.argv) == 4:
        conan_install = "-o {} -o {} -pr {}".format(sys.argv[2], sys.argv[3], platform)
    else:
        conan_install = "-pr {}".format(platform)
    print("[conan] conan_install_command = {}".format(conan_install))
    return conan_install
def run_command(conan_install_command):
    return_num = os.system(conan_install_command)
    if return_num != 0:
       exit (1)
def conan_build(conan_install_command):
    print("[conan] conan install .. ltc/fanya {}".format(conan_install_command))
    run_command("conan install .. ltc/fanya {}".format(conan_install_command))
    run_command("conan build ..")
    run_command("conan package  ..")
    run_command("conan export-pkg .. ltc/fanya {} -f".format(conan_install_command))
    # run_command("conan upload fanya-protocol/1.4.43ltc@fanya/test --all -r conan-test")

def build_start():
    check_and_make_dir(build_dir)  
    os.chdir(build_dir)
    conan_install_command = conan_install_argument(platform)
    conan_build(conan_install_command)

#----------------------run-------------------------------------  
if __name__ == "__main__":
    build_dir = 'build'
    platform = sys.argv[1]
    # USE_BOLEIDLC = sys.argv[2]
    if len(sys.argv) == 3 and sys.argv[2] == "coverage":
        compile()
    else:
        build_start()