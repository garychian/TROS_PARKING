#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace transaction {

/*! @brief base class to wrap received msg, with timestamp */
class WithTimestamp {
 public:
    WithTimestamp(uint64_t timestamp) : timestamp(timestamp) {}
    virtual ~WithTimestamp() = default;

    uint64_t timestamp;
};
typedef std::shared_ptr<WithTimestamp> WithTimestampPtr;

/*! @brief template class to wrap received msg */
template <typename T>
class DataWithTimestampT final : public WithTimestamp {
 public:
    DataWithTimestampT(std::shared_ptr<T> data, uint64_t timestamp)
        : data(data), WithTimestamp(timestamp) {}

    std::shared_ptr<T> data;
};

/*! @brief configure for each message */
struct TransactionMsgSyncConf {
    enum Requirement {
        REQUIRED = 0,
        OPTIONAL = 1,
        IGNORE = 2,
    };
    TransactionMsgSyncConf() {}

    std::string topic;    // unique tag name
    uint32_t queue_size;  // max queue size
    // timestamp tolerance in nanosecond
    uint32_t timestamp_ns_tolerance;
    Requirement requirement;
};

/*! @brief thread safe ... maybe */
class TransactionMsgQueue {
 public:
    TransactionMsgQueue(TransactionMsgSyncConf &conf) : conf_(conf) {}
    TransactionMsgQueue(const TransactionMsgQueue &other)
        : conf_(other.conf_) {}
    virtual ~TransactionMsgQueue() = default;

    template <typename T>
    void Push(std::shared_ptr<T> &data, uint64_t timestamp) {
        std::lock_guard<std::mutex> lock(consume_mutex_);
        queue_.push(std::make_shared<DataWithTimestampT<T>>(data, timestamp));
        if (queue_.size() > conf_.queue_size) {
            queue_.pop();
        }
    }

    inline bool Empty() {
        std::lock_guard<std::mutex> lock(consume_mutex_);
        return queue_.empty();
    }

    /*!
        @brief Get shared_ptr of element which matched the given timestamp
        @brief in given mistake_ns range.
        @param ts_ns: timestamp in nanosecond
        @param mistake_ns: tolerance in nanosecond
        @return shared_ptr of matched element or nullptr if no matched element
    */
    WithTimestampPtr SyncTimestamp(const uint64_t &ts_ns,
                                   uint64_t mistake_ns = 0);

    /*!
        @return first element of nullptr if queue is empty
    */
    inline std::shared_ptr<WithTimestamp> Front() {
        std::lock_guard<std::mutex> lock(consume_mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        return queue_.front();
    }

    /*!
        @return timestamp of first element or 0 if queue is empty
    */
    inline uint64_t FrontTimestamp() {
        std::lock_guard<std::mutex> lock(consume_mutex_);
        if (queue_.empty()) {
            return 0;
        }
        return queue_.front()->timestamp;
    }

    inline void Pop() {
        std::lock_guard<std::mutex> lock(consume_mutex_);
        queue_.pop();
    }
    inline bool Required() {
        return conf_.requirement == TransactionMsgSyncConf::REQUIRED;
    }
    inline bool Ignorable() {
        return conf_.requirement == TransactionMsgSyncConf::IGNORE;
    }
    inline const std::string &Topic() const { return conf_.topic; }
    inline size_t MaxTimeDiff() { return conf_.timestamp_ns_tolerance; }

 protected:
    TransactionMsgSyncConf conf_;
    std::mutex consume_mutex_;
    std::queue<WithTimestampPtr> queue_;
};

class TransactionSyncedMsg {
 public:
    TransactionSyncedMsg() = default;
    TransactionSyncedMsg(uint64_t timestamp) : timestamp_(timestamp) {}
    ~TransactionSyncedMsg() = default;

    uint64_t &Timestamp() { return timestamp_; }
    const uint64_t Timestamp() const { return timestamp_; }

    inline void Set(const std::string &topic, WithTimestampPtr data) {
        synced_msgs[topic] = data;
    }

    template <typename T>
    std::shared_ptr<T> Get(const std::string &topic) const {
        auto itr = synced_msgs.find(topic);
        if (itr == synced_msgs.end()) {
            AD_LWARN(TransactionSyncedMsg)
                << "topic " << topic << " not found in synced_msgs";
            return nullptr;
        }
        auto transaction_msg =
            std::dynamic_pointer_cast<DataWithTimestampT<T>>(itr->second);
        if (transaction_msg == nullptr) {
            AD_LERROR(TransactionSyncedMsg)
                << "topic " << topic << " is not type of " << typeid(T).name();
            return nullptr;
        }
        return transaction_msg->data;
    }

 private:
    uint64_t timestamp_;
    std::map<std::string, WithTimestampPtr> synced_msgs;
};

class TimeSync final {
 public:
    void Register(std::initializer_list<std::string> tags,
                  uint32_t queue_size,
                  uint32_t timestamp_ns_tolerance,
                  TransactionMsgSyncConf::Requirement requirement =
                      TransactionMsgSyncConf::REQUIRED) {
        for (auto &tag : tags) {
            Register(tag, queue_size, timestamp_ns_tolerance, requirement);
        }
    }

    /*!
        @brief Register a message to be synchronized.
        @param tag: unique tag name
        @param queue_size: max queue size
        @param timestamp_ns_tolerance: nanosecond tolerance when syncing
        @param requirement: REQUIRED, OPTIONAL, IGNORE
        @return void
     */
    void Register(const std::string &tag,
                  uint32_t queue_size,
                  uint32_t timestamp_ns_tolerance,
                  TransactionMsgSyncConf::Requirement requirement =
                      TransactionMsgSyncConf::REQUIRED);

    template <typename T>
    bool Push(const std::string &tag,
              uint64_t timestamp,
              std::shared_ptr<T> data) {
        auto itr = datas_.find(tag);
        if (itr == datas_.end()) {
            AD_LERROR(Transaction::TimeSync)
                << "tag " << tag << " not registered";
            return false;
        }
        itr->second.Push(data, timestamp);
        return true;
    }

    /*!
        @brief Get synced data. Return nullptr if no synced data
        @return shared_ptr of TransactionSyncedMsg; nullptr if no synced data
    */
    std::shared_ptr<TransactionSyncedMsg> TryGetSyncData();

 private:
    std::map<std::string, TransactionMsgQueue> datas_;
    std::mutex sync_mutex_;
};

}  // namespace transaction
}  // namespace senseAD