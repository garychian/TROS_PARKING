/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Fan Xuzeng <fanxuzeng@sensetime.com>
 */

#ifndef _MATRIXSDK_MCOMMON_MMETA_JSONHELPER_HPP_
#define _MATRIXSDK_MCOMMON_MMETA_JSONHELPER_HPP_

#include "MJson.hpp"

enum CtrlType { REQUIRED, OPTIANAL };

class JsonHelper {
public:
    static bool GetJsonRootFromFile(const std::string& config_file, MJson& root);
    static bool GetJsonRoot(const std::string param_json, MJson& root);
    static bool GetIfJsonExist(const MJson& root, MJson& target, std::string name, CtrlType t);
    static bool GetJson(const MJson& root, MJson& target, std::string name, CtrlType t);
    static bool GetJsonObj(const MJson& root, std::string name, MJson::object& val, CtrlType t);
    static bool GetJsonInt(const MJson& root, std::string name, int& val, CtrlType t);
    static bool GetJsonUint(const MJson& root, std::string name, unsigned int& val, CtrlType t);
    static bool GetJsonDouble(const MJson& root, std::string name, double& val, CtrlType t);
    static bool GetJsonFloat(const MJson& root, std::string name, float& val, CtrlType t);
    static bool GetJsonStr(const MJson& root, std::string name, std::string& val, CtrlType t);
    static bool GetJsonBool(const MJson& root, std::string name, bool& val, CtrlType t);

    static bool GetJsonIntArr1D(const MJson& root,
                                std::string name,
                                std::vector< int >& val,
                                CtrlType t,
                                int expect_size);
    static bool GetJsonDoubleArr1D(const MJson& root,
                                   std::string name,
                                   std::vector< double >& val,
                                   CtrlType t,
                                   int expect_size);
    static bool GetJsonFloatArr1D(const MJson& root,
                                  std::string name,
                                  std::vector< float >& val,
                                  CtrlType t,
                                  int expect_size);
    static bool GetJsonStrArr1D(const MJson& root,
                                std::string name,
                                std::vector< std::string >& val,
                                CtrlType t,
                                int expect_size);
    static bool GetJsonBoolArr1D(const MJson& root,
                                 std::string name,
                                 std::vector< bool >& val,
                                 CtrlType t,
                                 int expect_size);

    static bool GetJsonIntArr2D(const MJson& root,
                                std::string name,
                                std::vector< std::vector< int > >& val,
                                CtrlType t);
    static bool GetJsonDoubleArr2D(const MJson& root,
                                   std::string name,
                                   std::vector< std::vector< double > >& val,
                                   CtrlType t);
    static bool GetJsonFloatArr2D(const MJson& root,
                                  std::string name,
                                  std::vector< std::vector< float > >& val,
                                  CtrlType t);
    static bool GetJsonStrArr2D(const MJson& root,
                                std::string name,
                                std::vector< std::vector< std::string > >& val,
                                CtrlType t);
    static bool GetJsonBoolArr2D(const MJson& root,
                                 std::string name,
                                 std::vector< std::vector< bool > >& val,
                                 CtrlType t);

    static bool GetJsonIntArr3D(const MJson& root,
                                std::string name,
                                std::vector< std::vector< std::vector< int > > >& val,
                                CtrlType t);
    static bool GetJsonDoubleArr3D(const MJson& root,
                                   std::string name,
                                   std::vector< std::vector< std::vector< double > > >& val,
                                   CtrlType t);
    static bool GetJsonFloatArr3D(const MJson& root,
                                  std::string name,
                                  std::vector< std::vector< std::vector< float > > >& val,
                                  CtrlType t);
    static bool GetJsonStrArr3D(const MJson& root,
                                std::string name,
                                std::vector< std::vector< std::vector< std::string > > >& val,
                                CtrlType t);
    static bool GetJsonBoolArr3D(const MJson& root,
                                 std::string name,
                                 std::vector< std::vector< std::vector< bool > > >& val,
                                 CtrlType t);
};

#endif // _MATRIXSDK_MCOMMON_MMETA_JSONHELPER_HPP_
