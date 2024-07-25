#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <sys/stat.h>
#include <cstdio>

namespace senseAD {

inline int getBinSize(const std::string& path) {
    int size = 0;
    std::ifstream infile(path, std::ifstream::binary);
    if (!infile.is_open()) {
        std::cerr << "[ERROR] open ipm_stitchmap bin file: " << path.c_str()
                  << " failed." << std::endl;
        return 0;
    }
    infile.seekg(0, infile.end);
    size = infile.tellg();
    infile.seekg(0, infile.beg);

    infile.close();
    return size;
}

inline std::string readText(const std::string& file) {
    std::ifstream infile;
    infile.open(file.data());
    assert(infile.is_open());
    std::stringstream buffer;
    buffer << infile.rdbuf();
    std::string contents(buffer.str());
    infile.close();
    return contents;
}

namespace fs {  // filesystem

inline bool isDir(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 && info.st_mode & S_IFDIR;
}

inline bool renameFile(const std::string& old_path, const std::string& new_path) {
    return rename(old_path.c_str(), new_path.c_str()) == 0;
}

inline bool makeDirTree(const std::string& path) {
    int status;
    std::string cmd = "mkdir -p " + path;
    status = system(cmd.c_str());
    return status != -1;
}

inline bool copyFile(const std::string& src_path, const std::string& tgt_path) {
    std::ifstream src(src_path, std::ios::binary);
    if (!src.is_open()) {
        return false;
    }
    std::ofstream tgt(tgt_path, std::ios::binary);
    tgt << src.rdbuf();
    src.close();
    tgt.close();
    return true;
}

}


}  // namespace senseAD