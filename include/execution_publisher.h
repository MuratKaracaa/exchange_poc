#pragma once

#include <memory>
#include <vector>
#include <atomic>
#include "concurrentqueue.h"
#include "BS_thread_pool.hpp"
#include "execution_report_data.h"
#include "session_set.h"
#include "ExecutionReport.h"
#include "MarketDataSnapshotFullRefresh.h"

class ExecutionPublisher
{
private:
    moodycamel::ConcurrentQueue<std::shared_ptr<NotificationBase>> &notification_queue;
    SessionSet &session_set;
    std::unique_ptr<BS::thread_pool> thread_pool;

    size_t batch_size;
    size_t thread_count;
    std::vector<moodycamel::ConsumerToken> consumer_tokens;

    void process_batch(const std::vector<std::shared_ptr<NotificationBase>> &notifications, size_t count);
    void process_execution_report(const ExecutionReportData &report);
    void process_market_data_update(const MarketDataUpdate &update);

public:
    ExecutionPublisher(moodycamel::ConcurrentQueue<std::shared_ptr<NotificationBase>> &notification_queue_,
                       SessionSet &session_set_,
                       size_t thread_count_ = 4,
                       size_t batch_size_ = 100);
    ~ExecutionPublisher();

    void start();
    void stop();

    void publish_execution_report(const ExecutionReportData &report);

    void publish_market_data(const MarketDataUpdate &update);
};