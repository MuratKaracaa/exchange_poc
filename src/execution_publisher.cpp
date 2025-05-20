#include "execution_publisher.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>
#include <thread>
#include "Session.h"
#include "constants.h"

ExecutionPublisher::ExecutionPublisher(moodycamel::ConcurrentQueue<std::shared_ptr<NotificationBase>> &notification_queue_,
                                       SessionSet &session_set_,
                                       size_t thread_count_,
                                       size_t batch_size_)
    : notification_queue(notification_queue_),
      session_set(session_set_),
      thread_count(thread_count_),
      batch_size(batch_size_)
{
    consumer_tokens.reserve(thread_count);

    for (size_t i = 0; i < thread_count; ++i)
    {
        consumer_tokens.emplace_back(notification_queue);
    }

    thread_pool = std::make_unique<BS::thread_pool>(thread_count);
}

ExecutionPublisher::~ExecutionPublisher()
{
    stop();
}

void ExecutionPublisher::start()
{
    execution_publisher_running.store(true, std::memory_order_relaxed);

    for (size_t i = 0; i < thread_count; ++i)
    {
        thread_pool->submit_task([this, i]()
                                 {
            moodycamel::ConsumerToken& token = consumer_tokens[i];
            
            std::vector<std::shared_ptr<NotificationBase>> notifications(batch_size);

            while (execution_publisher_running.load(std::memory_order_relaxed))
            {
                size_t count = notification_queue.try_dequeue_bulk(token, notifications.data(), batch_size);

                if (count > 0) {
                    process_batch(notifications, count);
                } else {
                    std::this_thread::yield();
                }
            }
            
            size_t remaining_count;
            do {
                remaining_count = notification_queue.try_dequeue_bulk(token, notifications.data(), batch_size);
                if (remaining_count > 0) {
                    process_batch(notifications, remaining_count);
                }
            } while (remaining_count > 0); });
    }
}

void ExecutionPublisher::stop()
{
    execution_publisher_running.store(false, std::memory_order_relaxed);

    std::shared_ptr<NotificationBase> notification;
    while (notification_queue.try_dequeue(notification))
    {
        if (notification)
        {
            switch (notification->type)
            {
            case NotificationType::EXECUTION_REPORT:
            {
                const auto *report = dynamic_cast<const ExecutionReportData *>(notification.get());
                if (report)
                {
                    process_execution_report(*report);
                }
                break;
            }
            case NotificationType::MARKET_DATA_UPDATE:
            {
                const auto *update = dynamic_cast<const MarketDataUpdate *>(notification.get());
                if (update)
                {
                    process_market_data_update(*update);
                }
                break;
            }
            }
        }
    }

    thread_pool->wait();
}

void ExecutionPublisher::process_batch(const std::vector<std::shared_ptr<NotificationBase>> &notifications, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        const std::shared_ptr<NotificationBase> &notification = notifications[i];

        if (!notification)
            continue;

        switch (notification->type)
        {
        case NotificationType::EXECUTION_REPORT:
        {
            const auto *report = dynamic_cast<const ExecutionReportData *>(notification.get());
            if (report)
            {
                process_execution_report(*report);
            }
            break;
        }
        case NotificationType::MARKET_DATA_UPDATE:
        {
            const auto *update = dynamic_cast<const MarketDataUpdate *>(notification.get());
            if (update)
            {
                process_market_data_update(*update);
            }
            break;
        }
        }
    }
}

void ExecutionPublisher::process_execution_report(const ExecutionReportData &report)
{
    if (!session_set.contains(report.client_session_id))
    {
        return;
    }

    try
    {
        FIX42::ExecutionReport message(
            FIX::OrderID(report.order_id),
            FIX::ExecID(report.order_id),
            FIX::ExecTransType('0'),
            FIX::ExecType('2'),
            FIX::OrdStatus('2'),
            FIX::Symbol(report.symbol),
            FIX::Side('1'),
            FIX::LeavesQty(0),
            FIX::CumQty(report.executed_qty),
            FIX::AvgPx(report.execution_price));

        message.set(FIX::LastShares(report.executed_qty));
        message.set(FIX::LastPx(report.execution_price));
        message.set(FIX::TransactTime(FIX::UTCTIMESTAMP()));

        FIX::Session::sendToTarget(message, report.client_session_id);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error sending execution report: " << e.what() << std::endl;
    }
}

void ExecutionPublisher::process_market_data_update(const MarketDataUpdate &update)
{
    try
    {
        FIX42::MarketDataSnapshotFullRefresh message(FIX::Symbol(update.symbol));

        FIX42::MarketDataSnapshotFullRefresh::NoMDEntries tradeGroup;
        tradeGroup.set(FIX::MDEntryType('2')); // Trade
        tradeGroup.set(FIX::MDEntryPx(update.last_trade_price));
        tradeGroup.set(FIX::MDEntrySize(update.last_trade_quantity));
        message.addGroup(tradeGroup);

        message.set(FIX::TotalVolumeTraded(update.total_volume));

        session_set.forEach([&message](const FIX::SessionID &sessionID)
                            {
            try {
                FIX::Session::sendToTarget(message, sessionID);
            } catch (const std::exception& e) {
                std::cerr << "Error sending market data to session " << sessionID 
                          << ": " << e.what() << std::endl;
            } });
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error creating market data message: " << e.what() << std::endl;
    }
}

void ExecutionPublisher::publish_execution_report(const ExecutionReportData &report)
{
    auto report_ptr = std::make_shared<ExecutionReportData>(report);
    notification_queue.enqueue(report_ptr);
}

void ExecutionPublisher::publish_market_data(const MarketDataUpdate &update)
{
    auto update_ptr = std::make_shared<MarketDataUpdate>(update);
    notification_queue.enqueue(update_ptr);
}