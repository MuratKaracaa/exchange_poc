#pragma once

#include <vector>
#include <atomic>
#include <memory>
#include "concurrentqueue.h"
#include "order.h"
#include "BS_thread_pool.hpp"
#include "stock.h"
#include "ankerl/unordered_dense.h"

class OrderConsumerPool {
public:
    OrderConsumerPool(moodycamel::ConcurrentQueue<Order>& order_queue_, ankerl::unordered_dense::map<std::string, Stock>& stock_map_,size_t thread_count_, size_t batch_size_);
    
    ~OrderConsumerPool();
    
    void start();
    
    void stop();
    
    OrderConsumerPool(const OrderConsumerPool&) = delete;
    OrderConsumerPool& operator=(const OrderConsumerPool&) = delete;
    OrderConsumerPool(OrderConsumerPool&&) = delete;
    OrderConsumerPool& operator=(OrderConsumerPool&&) = delete;

private:
    void process_batch(const std::vector<Order>& orders, size_t count);
    
    void process_order(const Order& order);

    moodycamel::ConcurrentQueue<Order>& order_queue;

    ankerl::unordered_dense::map<std::string, Stock>& stock_map;
    
    std::unique_ptr<BS::thread_pool> thread_pool;
    
    size_t batch_size;
    size_t thread_count;
    
    std::vector<moodycamel::ConsumerToken> consumer_tokens;
};