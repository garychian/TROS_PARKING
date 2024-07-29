#ifndef __INCLUDE_COMMON_IO_HPP__
#define __INCLUDE_COMMON_IO_HPP__
#include <fcntl.h>
#include <proto/perception_camera.pb.h>
#include <stdint.h>

#include <fstream>
#include <iostream>
#include <string>

#include "common/error.hpp"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/message.h"
#include "google/protobuf/text_format.h"

namespace senseAD {
namespace perception {
namespace camera {

using google::protobuf::Message;
using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;

#define BIAS 19

inline apStatus_t ReadTxtProtoFromFile(const char* file_path,
                                       Message* proto,
                                       bool logerror = true) {
    AP_LINFO(ReadFileToString) << "start read file: " << file_path;
    int fd = open(file_path, O_RDONLY);
    if (-1 == fd) {
        if (logerror) {
            AP_LERROR(ReadTxtProtoFromFile) << "file not found::" << file_path;
            AP_CHECK_ERROR(AP_FILE_NOT_FOUND);
        }
        return AP_FILE_NOT_FOUND;
    }
    FileInputStream* input = new FileInputStream(fd);
    bool success = google::protobuf::TextFormat::Parse(input, proto);
    delete input;
    close(fd);
    if (!success) {
        AP_LERROR(ReadTxtProtoFromFile) << "invalid file format::" << file_path;
        AP_CHECK_ERROR(AP_INVALID_FILE_FORMAT);
    }
    return AP_SUCCESS;
}

inline apStatus_t ReadBinProtoFromFile(const char* file_path,
                                       Message* proto,
                                       bool logerror = true) {
    std::ifstream infile;
    std::string input;
    infile.open(file_path, std::ios::in);
    if (!infile.is_open()) {
        if (logerror) {
            AP_LERROR(ReadBinProtoFromFile) << "invalid file " << file_path;
            AP_CHECK_ERROR(AP_FILE_NOT_FOUND);
        }
        return AP_FILE_NOT_FOUND;
    }
    std::stringstream buffer;
    buffer << infile.rdbuf();
    std::string contents(buffer.str());
    size_t n = 0;
    while (n < contents.size()) {
        char p = contents[n];
        char x;
        if (static_cast<int>(p) < BIAS) {
            x = static_cast<char>(255 - (static_cast<int>(p) + 256 - BIAS));
        } else {
            x = static_cast<char>(255 - (static_cast<int>(p) - BIAS));
        }
        input = input + static_cast<char>(x);
        n++;
    }
    infile.close();
    bool success = google::protobuf::TextFormat::ParseFromString(input, proto);
    if (!success) {
        AP_LERROR(ReadBinProtoFromFile) << "invalid file format::" << file_path;
        AP_CHECK_ERROR(AP_INVALID_FILE_FORMAT);
    }

    return AP_SUCCESS;
}

inline apStatus_t ReadProtoFromFile(const char* file_path,
                                    Message* proto,
                                    bool logerror = true) {
    if (nullptr == file_path || nullptr == proto) {
        AP_CHECK_ERROR(AP_NULL_PTR);
    }
    std::string str_file_path(file_path);
    std::string suffixStr =
        str_file_path.substr(str_file_path.find_last_of('.') + 1);
    if (suffixStr == "bin") {
        return ReadBinProtoFromFile(file_path, proto, logerror);
    } else {
        return ReadTxtProtoFromFile(file_path, proto, logerror);
    }
}

inline apStatus_t ReadProtoFromFile(const std::string& file_path,
                                    Message* proto,
                                    bool logerror = true) {
    if (nullptr == proto) {
        AP_CHECK_ERROR(AP_NULL_PTR);
    }
    return ReadProtoFromFile(file_path.c_str(), proto, logerror);
}

inline std::string JoinPath(std::string root, std::string path) {
    if (root.empty()) {
        AP_LDEBUG(JoinPath) << "root path empty, may relative path used.";
        return path;
    }
    if (path.empty()) {
        AP_LDEBUG(JoinPath) << "file path empty, please check your file path.";
    }
    return (root.back() == '/' ? root + path : root + '/' + path);
}

}  // namespace camera
}  // namespace perception
}  // namespace senseAD

#endif  // __INCLUDE_COMMON_IO_HPP__
