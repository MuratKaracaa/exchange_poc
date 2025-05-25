#pragma once

#include <vector>
#include <atomic>
#include <memory>
#include "concurrentqueue.h"
#include "order.h"
#include "BS_thread_pool.hpp"
#include "stock.h"
#include "ankerl/unordered_dense.h"
#include "execution_publisher.h"

class OrderConsumerPool
{
public:
    OrderConsumerPool(moodycamel::ConcurrentQueue<Order> &order_queue_,
                      ankerl::unordered_dense::map<std::string, Stock> &stock_map_,
                      ExecutionPublisher &execution_publisher_,
                      size_t thread_count_ = 8,
                      size_t batch_size_ = 100);

    ~OrderConsumerPool();

    void start();

    void stop();

    OrderConsumerPool(const OrderConsumerPool &) = delete;
    OrderConsumerPool &operator=(const OrderConsumerPool &) = delete;
    OrderConsumerPool(OrderConsumerPool &&) = delete;
    OrderConsumerPool &operator=(OrderConsumerPool &&) = delete;

private:
    void process_batch(std::vector<Order> &orders, size_t count);

    void process_order(Order &&order);

    void send_trade_notifications(const Order &order, const Order &matched_order,
                                  double execution_price, int execution_quantity,
                                  const std::string &timestamp);

    std::string generate_timestamp();

    moodycamel::ConcurrentQueue<Order> &order_queue;

    ankerl::unordered_dense::map<std::string, Stock> &stock_map;

    ExecutionPublisher &execution_publisher;

    std::unique_ptr<BS::thread_pool> thread_pool;

    size_t batch_size;
    size_t thread_count;

    std::vector<moodycamel::ConsumerToken> consumer_tokens;
};