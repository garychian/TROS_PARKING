#pragma once

#include <set>
#include "ad_rscl/ad_rscl.h"

namespace senseAD {
namespace avp_utils {

using senseAD::rscl::comm::SubscriberBase;

/**
 * @brief 订阅管理器，基于业务状态管理订阅的激活和禁用
 * @todo 支持timer
 */
class SubscriberManager {
 public:
    void Clear();

    /**
     * @brief 非必须，注册所有需要管理的订阅，主要影响OnDefaultActivateAll()
     * @brief 注册时会禁用所有订阅，等待状态切换激活
     */
    template <typename... Subscribers>
    SubscriberManager& Manage(Subscribers... subs);

    /**
     * @brief 默认激活所有Manage()添加的订阅
     */
    SubscriberManager& OnDefaultActivateAll();
    /**
     * @brief 指定默认激活的订阅，对所有未设置的状态都生效
     */
    template <typename... Subscribers>
    SubscriberManager& OnDefaultActivate(Subscribers... subs);

    inline SubscriberManager& OnStageActivate(int stage) {
        subscribers_onstage_[stage].clear();
        return *this;
    }
    /**
     * @brief 在某个状态下激活指定的订阅，留空表示不激活任何订阅
     */
    template <typename... Subscribers>
    SubscriberManager& OnStageActivate(int stage, Subscribers... subs);

    /**
     * @brief 在某些状态下激活指定的订阅，在该状态设置（或者默认）的基础上添加
     */
    template <typename... Stages>
    SubscriberManager& ActivateOnStage(SubscriberBase::Ptr, Stages... stages);

    /**
     * @brief 在某些状态下禁用指定的订阅，在该状态设置（或者默认）的基础上添加
     */
    template <typename... Stages>
    SubscriberManager& DeactivateOnStage(SubscriberBase::Ptr, Stages... stages);

    /**
     * @brief 设置当前状态，会触发订阅的激活和禁用
     * @param stage 要切换的状态
     */
    void SetCurrentStage(int stage);

    /**
     * @brief 内部启动线程接收状态机并自动触发状态切换，
     * @brief 不需要主动调用SetCurrentStage
     * @warning 未实现
     * @param topic 状态机的topic
     */
    bool SetupStageSubscriber(const std::string& topic);

 private:
    const std::set<SubscriberBase*>& GetActivateSubscribers(int stage);

    // default stage is -1
    static constexpr int DEFAULT_STAGE = -1;
    int current_stage_ = -1;

    // 所有状态订阅设置
    std::unordered_map<int, std::set<SubscriberBase*>> subscribers_onstage_;
    // 所有添加过的订阅，包括Manage()和OnXXX()添加的
    std::set<SubscriberBase*> all_subscribers_;
};

template <typename... Subscribers>
SubscriberManager& SubscriberManager::Manage(Subscribers... subs) {
    all_subscribers_.clear();
    for (auto& sub : {subs...}) {
        // Disable all subscribers on startup
        if (sub->IsEnabled()) sub->Disable();
        all_subscribers_.insert(sub.get());
    }
    return *this;
}

template <typename... Subscribers>
SubscriberManager& SubscriberManager::OnDefaultActivate(Subscribers... subs) {
    subscribers_onstage_[DEFAULT_STAGE].clear();
    for (auto& sub : {subs...}) {
        subscribers_onstage_[DEFAULT_STAGE].insert(sub.get());
        all_subscribers_.insert(sub.get());
    }
    return *this;
}

template <typename... Subscribers>
SubscriberManager& SubscriberManager::OnStageActivate(int stage,
                                                      Subscribers... subs) {
    subscribers_onstage_[stage].clear();
    for (auto& sub : {subs...}) {
        subscribers_onstage_[stage].insert(sub.get());
        all_subscribers_.insert(sub.get());
    }
    return *this;
}

template <typename... Stages>
SubscriberManager& SubscriberManager::ActivateOnStage(SubscriberBase::Ptr sub,
                                                      Stages... stages) {
    for (auto stage : {stages...}) {
        if (subscribers_onstage_.find(stage) == subscribers_onstage_.end()) {
            subscribers_onstage_[stage] = subscribers_onstage_[DEFAULT_STAGE];
        }
        subscribers_onstage_[stage].insert(sub.get());
    }
    all_subscribers_.insert(sub.get());
    return *this;
}

template <typename... Stages>
SubscriberManager& SubscriberManager::DeactivateOnStage(SubscriberBase::Ptr sub,
                                                        Stages... stages) {
    for (auto stage : {stages...}) {
        if (subscribers_onstage_.find(stage) == subscribers_onstage_.end()) {
            subscribers_onstage_[stage] = subscribers_onstage_[DEFAULT_STAGE];
        }
        subscribers_onstage_[stage].erase(sub.get());
    }
    all_subscribers_.insert(sub.get());
    return *this;
}

}  // namespace avp_utils
}  // namespace senseAD