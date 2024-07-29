/*
 * Copyright (C) 2021-2023 by SenseTime Group Limited. All rights reserved.
 * Chen Zhenan <chenzhenan@senseauto.com>
 */

#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <ad_rscl/ad_rscl.h>
#include <ad_msg_idl/avp_diag/avp_diag.capnp.h>
#include "avp_diag/avp_diag.h"

namespace senseAD {
namespace avp_diag {

class ExceptionPublisher {
 public:
    using Ptr = std::shared_ptr<ExceptionPublisher>;

    ExceptionPublisher(rscl::comm::Node *node, int report_interval = 1000) {
        exception_pub_ = node->CreatePublisher<msg::avp_diag::DiagExceptionList>(
            "/avp_diag/exception_report");
        report_timer_ = std::make_unique<senseAD::rscl::task::TimerTask>(
            report_interval, &ExceptionPublisher::onTimer, this);
        report_timer_->Start();
    }

    void AddException(const DiagException &exception) {
        std::lock_guard<std::mutex> lock(mutex_);
        exceptions_.emplace_back(exception);
    }

 private:
    std::unique_ptr<rscl::task::TimerTask> report_timer_;
    std::vector<DiagException> exceptions_;
    std::mutex mutex_;
    rscl::comm::Publisher<msg::avp_diag::DiagExceptionList>::Ptr exception_pub_;

    void onTimer()  {
        std::lock_guard<std::mutex> lock(mutex_);

        if (exceptions_.size() == 0) {
            return;
        }

        auto msg = exception_pub_->NewSendMsg();
        auto msg_builder = msg->MsgBuilder();
        uint64_t now = senseAD::rscl::time::Time::Now().ToNanosecond();
        auto header = msg_builder.getHeader();
        header.getTime().setNanoSec(now);
        auto exception_list = msg_builder.initExceptions(exceptions_.size());
        for (size_t i = 0; i < exceptions_.size(); i++) {
            exception_list[i].setTimestampNs(exceptions_[i].timestamp_ns);
            exception_list[i].setCode(exceptions_[i].code);
            exception_list[i].setMessage(exceptions_[i].message);
        }

        AD_LINFO(ExceptionPublisher) << "Publish exception timestamp_ns: " << now;
        exception_pub_->Publish(msg);

        exceptions_.clear();
    }
};

#define AVP_DIAG_CHECK_TRUE_PUBLISH(f, code, msg, time, puber)              \
    {                                                                       \
        if (f && puber) {                                                   \
            puber->AddException(                                            \
                senseAD::avp_diag::DiagException(time, code, msg));         \
            AD_LWARN(AVP_DIAG) << "Get diag: " << code << " . Msg: " << msg \
                               << " . At: " << time;                        \
        }                                                                   \
    }

#define AVP_DIAG_CHECK_TRUE_PUBLISH_NOW(f, code, msg, puber)                  \
    {                                                                         \
        if (f && puber) {                                                     \
            puber->AddException(senseAD::avp_diag::DiagException(             \
                senseAD::rscl::time::Time::Now().ToNanosecond(), code, msg)); \
            AD_LWARN(AVP_DIAG) << "Get diag: " << code << " . Msg: " << msg;  \
        }                                                                     \
    }

}  // namespace avp_diag
}  // namespace senseAD
