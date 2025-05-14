#include "order_consumer_pool.h"
#include <thread>
#include <iostream>
#include <constants.h>
#include "stock.h"

OrderConsumerPool::OrderConsumerPool(moodycamel::ConcurrentQueue<Order> &queue_, ankerl::unordered_dense::map<std::string, Stock> &stock_map_, size_t thread_count_, size_t batch_size_)
    : order_queue(queue_), stock_map(stock_map_), batch_size(batch_size_), thread_count(thread_count_)
{

    for (size_t i = 0; i < thread_count; ++i)
    {
        consumer_tokens.emplace_back(order_queue);
    }

    thread_pool = std::make_unique<BS::thread_pool>(thread_count);
}

OrderConsumerPool::~OrderConsumerPool()
{
    stop();
}

void OrderConsumerPool::start()
{

    for (size_t i = 0; i < thread_count; ++i)
    {
        thread_pool->submit_task([this, i]()
                                 {
            moodycamel::ConsumerToken& token = consumer_tokens[i];
            
            std::vector<Order> orders(batch_size);
            
            while (global_processing_state.load(std::memory_order_relaxed)) {
                
                size_t count = order_queue.try_dequeue_bulk(token, orders.data(), batch_size);
                
                if (count > 0) {
                    process_batch(orders, count);
                } else {
                    std::this_thread::yield();
                }
            } });
    }
}

void OrderConsumerPool::stop()
{
    thread_pool->wait();
}

void OrderConsumerPool::process_batch(const std::vector<Order> &orders, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        process_order(orders[i]);
    }
}

void OrderConsumerPool::process_order(const Order &incoming_order)
{
    Stock &stock = stock_map[incoming_order.get_symbol()];
    OrderBook &order_book = stock.get_order_book();
    int remaining_incoming_order_quantity = incoming_order.get_quantity();

    if (incoming_order.get_order_type() == OrderType::MARKET)
    {
        while (remaining_incoming_order_quantity)
        {
            std::optional<Order> next_sell_order_optional = incoming_order.get_order_side() == OrderSide::BUY ? order_book.get_top_sell_order() : order_book.get_top_buy_order();
            if (!next_sell_order_optional)
            {
                order_book.add_market_order(incoming_order);
                break;
            }
            else
            {
                Order &next_sell_order = *next_sell_order_optional;
                int remaining_temp = remaining_incoming_order_quantity;
                remaining_incoming_order_quantity = std::max(0, remaining_incoming_order_quantity - next_sell_order.get_quantity());
                if (next_sell_order.get_quantity() - remaining_temp)
                {
                    next_sell_order.reduce_quantity(remaining_temp);
                    order_book.add_market_order(next_sell_order);
                }
            }
        }
    }
}