/*
 * @copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * @brief protobuf interface.
 * @author Geng Menglin <gengmenglin@senseauto.com>
 * @date since 2022.4.6
 */

#pragma once

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using google::protobuf::Message;
using google::protobuf::io::FileInputStream;

namespace Perspective {

// parse parameters from prototxt to current running lib.

inline bool ParseFromPrototxt(const std::string filename, Message* proto) {
    std::ifstream istr(filename, std::ios_base::in);
    if (!istr.is_open()) {
        std::cout << "could not open file:" << filename << "\n";
        return false;
    }
    std::stringstream str_stream;
    str_stream << istr.rdbuf();

    std::string str_test = str_stream.str();
    if (!google::protobuf::TextFormat::ParseFromString(str_test, proto)) {
        std::cout << "could not parse the file:" << filename
                  << " as a prototxt file";
        return false;
    }
    istr.close();
    return true;
}

}  // namespace Perspective