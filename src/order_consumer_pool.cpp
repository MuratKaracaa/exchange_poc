#include "order_consumer_pool.h"
#include <thread>
#include <iostream>
#include <constants.h>
#include "stock.h"

OrderConsumerPool::OrderConsumerPool(moodycamel::ConcurrentQueue<Order>& queue_,ankerl::unordered_dense::map<std::string, Stock>& stock_map_ ,size_t thread_count_,size_t batch_size_)
    : order_queue(queue_), stock_map(stock_map_) ,batch_size(batch_size_), thread_count(thread_count_) {
    
    for (size_t i = 0; i < thread_count; ++i) {
        consumer_tokens.emplace_back(order_queue);
    }
    
    thread_pool = std::make_unique<BS::thread_pool>(thread_count);
}

OrderConsumerPool::~OrderConsumerPool() {
    stop();
}

void OrderConsumerPool::start() {
    
    for (size_t i = 0; i < thread_count; ++i) {
        thread_pool->submit_task([this, i]() {
            moodycamel::ConsumerToken& token = consumer_tokens[i];
            
            std::vector<Order> orders(batch_size);
            
            while (global_processing_state.load(std::memory_order_relaxed)) {
                
                size_t count = order_queue.try_dequeue_bulk(token, orders.data(), batch_size);
                
                if (count > 0) {
                    process_batch(orders, count);
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
}

void OrderConsumerPool::stop() {
    thread_pool->wait();
}

void OrderConsumerPool::process_batch(const std::vector<Order>& orders, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        process_order(orders[i]);
    }
}

void OrderConsumerPool::process_order(const Order& order) {

}