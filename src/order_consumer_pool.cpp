#include "order_consumer_pool.h"
#include <thread>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "constants.h"
#include "boost/date_time/posix_time/posix_time.hpp"

OrderConsumerPool::OrderConsumerPool(moodycamel::ConcurrentQueue<Order> &queue_, ankerl::unordered_dense::map<std::string, Stock> &stock_map_, ExecutionPublisher &execution_publisher_, size_t thread_count_, size_t batch_size_)
    : order_queue(queue_), stock_map(stock_map_), execution_publisher(execution_publisher_), batch_size(batch_size_), thread_count(thread_count_)
{
    consumer_tokens.reserve(thread_count);

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

std::string OrderConsumerPool::generate_timestamp()
{
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    std::string iso_string = boost::posix_time::to_iso_extended_string(now);

    size_t comma_pos = iso_string.find(',');
    if (comma_pos != std::string::npos)
    {
        iso_string[comma_pos] = '.';
        if (iso_string.length() > comma_pos + 4)
        {
            iso_string = iso_string.substr(0, comma_pos + 4);
        }
    }

    return iso_string + "Z";
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
            }
            
            size_t remaining_count;
            do {
                remaining_count = order_queue.try_dequeue_bulk(token, orders.data(), batch_size);
                if (remaining_count > 0) {
                    process_batch(orders, remaining_count);
                }
            } while (remaining_count > 0); });
    }
}

void OrderConsumerPool::stop()
{
    global_processing_state.store(false, std::memory_order_relaxed);

    thread_pool->wait();

    Order order;
    while (order_queue.try_dequeue(order))
    {
        process_order(std::move(order));
    }

    consumer_tokens.clear();
}

void OrderConsumerPool::process_batch(std::vector<Order> &orders, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        process_order(std::move(orders[i]));
    }
}

void OrderConsumerPool::process_order(Order &&incoming_order)
{
    Stock &stock = stock_map[incoming_order.get_symbol()];
    OrderBook &order_book = stock.get_order_book();
    int remaining_incoming_order_quantity = incoming_order.get_quantity();

    if (incoming_order.get_order_type() == OrderType::MARKET)
    {
        while (remaining_incoming_order_quantity > 0)
        {
            std::optional<Order> matching_order_optional =
                incoming_order.get_order_side() == OrderSide::BUY ? order_book.get_top_sell_order() : order_book.get_top_buy_order();

            if (!matching_order_optional)
            {
                order_book.add_market_order(std::move(incoming_order));
                break;
            }
            else
            {
                Order matching_order = std::move(*matching_order_optional);
                int executed_quantity = std::min(remaining_incoming_order_quantity, matching_order.get_quantity());
                double execution_price = matching_order.get_price();

                remaining_incoming_order_quantity -= executed_quantity;

                if (matching_order.get_quantity() - executed_quantity > 0)
                {
                    matching_order.reduce_quantity(executed_quantity);
                    order_book.add_market_order(std::move(matching_order));
                }

                std::string timestamp = generate_timestamp();

                stock.update_trading_info(execution_price, executed_quantity);
                send_trade_notifications(incoming_order, matching_order, execution_price, executed_quantity, timestamp);
            }
        }
    }
    else if (incoming_order.get_order_type() == OrderType::LIMIT)
    {
        while (remaining_incoming_order_quantity > 0)
        {
            std::optional<Order> matching_order_optional =
                incoming_order.get_order_side() == OrderSide::BUY ? order_book.get_top_sell_order() : order_book.get_top_buy_order();

            if (!matching_order_optional)
            {
                order_book.add_order(std::move(incoming_order));
                break;
            }

            Order matching_order = std::move(*matching_order_optional);

            if ((incoming_order.get_order_side() == OrderSide::BUY && incoming_order.get_price() < matching_order.get_price()) ||
                (incoming_order.get_order_side() == OrderSide::SELL && incoming_order.get_price() > matching_order.get_price()))
            {
                order_book.add_order(std::move(matching_order));
                order_book.add_order(std::move(incoming_order));
                break;
            }

            int executed_quantity = std::min(remaining_incoming_order_quantity, matching_order.get_quantity());
            double execution_price = matching_order.get_price();
            remaining_incoming_order_quantity -= executed_quantity;

            if (matching_order.get_quantity() - executed_quantity > 0)
            {
                matching_order.reduce_quantity(executed_quantity);
                order_book.add_order(std::move(matching_order));
            }

            std::string timestamp = generate_timestamp();

            stock.update_trading_info(execution_price, executed_quantity);
            send_trade_notifications(incoming_order, matching_order, execution_price, executed_quantity, timestamp);
        }
    }
}

void OrderConsumerPool::send_trade_notifications(const Order &order, const Order &matched_order,
                                                 double execution_price, int execution_quantity,
                                                 const std::string &timestamp)
{
    {
        ExecutionReportData report(
            matched_order.get_symbol(),
            matched_order.get_session_id(),
            matched_order.get_order_id(),
            execution_quantity,
            execution_price,
            timestamp);
        execution_publisher.publish_execution_report(report);
    }

    {
        MarketDataUpdate update(
            order.get_symbol(),
            execution_price,
            execution_quantity,
            stock_map[order.get_symbol()].get_trading_info().volume,
            timestamp);
        execution_publisher.publish_market_data(update);
    }
}