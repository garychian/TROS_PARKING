
#ifndef _MATRIXSDK_MCOMMON_MMETA_CORE_FACTORY_HPP_
#define _MATRIXSDK_MCOMMON_MMETA_CORE_FACTORY_HPP_
#include "common.h"
// #include "log.hpp"

// #include <core/Error.hpp>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

template < typename Base > class EXPORT_API CommonFactory {
public:
    using FactoryCreator = std::function< std::shared_ptr< Base >() >;
    using FactoryMap = std::unordered_map< std::string, FactoryCreator >;

    static CommonFactory& GetInstance() {
        static CommonFactory< Base > factory;
        return factory;
    }
    void Register(const std::string& key, FactoryCreator creator) {
        if (creator_map_.count(key)) {
            return;
        }
        creator_map_[key] = creator;
        return;
    }
    std::shared_ptr< Base > Create(const std::string& key) {
        if (!creator_map_.count(key)) {
            // SDK_LOG_ERROR("CommonFactory  type {} not register", key);
            return nullptr;
        }
        return creator_map_[key]();
    }

private:
    FactoryMap creator_map_;
};
template < typename Base > class CommonRegister {
public:
    using FactoryCreator = std::function< std::shared_ptr< Base >() >;
    CommonRegister(const std::string& type, FactoryCreator creator) {
        CommonFactory< Base >::GetInstance().Register(type, creator);
    }
};

#define REGISTER_COMMON_ENGINE(namespaces, key, classbase, classname)                     \
    namespace {                                                                           \
        std::shared_ptr< namespaces::classbase > creator_##key##_##classname() {          \
            return std::shared_ptr< namespaces::classbase >(new namespaces::classname()); \
        }                                                                                 \
        static CommonRegister< namespaces::classbase >                                    \
            g_creator_##key##_##classname(#key, creator_##key##_##classname);             \
    }

#endif
