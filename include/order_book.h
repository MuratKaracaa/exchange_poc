#pragma once

#include <boost/heap/d_ary_heap.hpp>
#include "order.h"
#include "order_side.h"
#include "order_type.h"
#include "constants.h"
#include "concurrentqueue.h"
#include <optional>

struct BuyOrderComparator
{
    bool operator()(const Order &a, const Order &b) const;
};

struct SellOrderComparator
{
    bool operator()(const Order &a, const Order &b) const;
};

typedef boost::heap::d_ary_heap<
    Order,
    boost::heap::arity<heap_arity>,
    boost::heap::compare<BuyOrderComparator>,
    boost::heap::mutable_<heap_mutability>>
    BuyOrderQueue;

typedef boost::heap::d_ary_heap<
    Order,
    boost::heap::arity<heap_arity>,
    boost::heap::compare<SellOrderComparator>,
    boost::heap::mutable_<heap_mutability>>
    SellOrderQueue;

class OrderBook
{
private:
    BuyOrderQueue buy_orders;
    SellOrderQueue sell_orders;
    moodycamel::ConcurrentQueue<Order> residual_market_buy_orders;
    moodycamel::ConcurrentQueue<Order> residual_market_sell_orders;

public:
    OrderBook() = default;

    void add_order(const Order &order);
    void add_order(Order &&order);
    void add_market_order(const Order &order);
    void add_market_order(Order &&order);
    std::optional<Order> get_next_market_buy_order();
    std::optional<Order> get_next_market_sell_order();
    std::optional<Order> get_top_buy_order();
    std::optional<Order> get_top_sell_order();
    bool get_can_match() const;
    size_t get_buy_orders_size() const;
    size_t get_sell_orders_size() const;
};