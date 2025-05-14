#include "order_book.h"
#include <algorithm>

bool BuyOrderComparator::operator()(const Order &a, const Order &b) const
{
    if (a.get_price() != b.get_price())
    {
        return a.get_price() < b.get_price();
    }
    return a.get_time_stamp() > b.get_time_stamp();
}

bool SellOrderComparator::operator()(const Order &a, const Order &b) const
{
    if (a.get_price() != b.get_price())
    {
        return a.get_price() > b.get_price();
    }
    return a.get_time_stamp() > b.get_time_stamp();
}

void OrderBook::add_order(const Order &order)
{
    if (order.get_order_side() == OrderSide::BUY)
    {
        buy_orders.push(order);
    }
    else
    {
        sell_orders.push(order);
    }
}

void OrderBook::add_market_order(const Order &order)
{
    if (order.get_order_side() == OrderSide::BUY)
    {
        residual_market_buy_orders.enqueue(order);
    }
    else
    {
        residual_market_sell_orders.enqueue(order);
    }
}

std::optional<Order> OrderBook::get_next_market_buy_order()
{
    Order order;
    if (residual_market_buy_orders.try_dequeue(order))
    {
        return order;
    }
    return std::nullopt;
}

std::optional<Order> OrderBook::get_next_market_sell_order()
{
    Order order;
    if (residual_market_sell_orders.try_dequeue(order))
    {
        return order;
    }
    return std::nullopt;
}

std::optional<Order> OrderBook::get_top_buy_order()
{
    if (buy_orders.empty())
    {
        return std::nullopt;
    }
    Order top = buy_orders.top();
    buy_orders.pop();
    return top;
}

std::optional<Order> OrderBook::get_top_sell_order()
{
    if (sell_orders.empty())
    {
        return std::nullopt;
    }
    Order top = sell_orders.top();
    sell_orders.pop();
    return top;
}

bool OrderBook::get_can_match() const
{
    if (buy_orders.empty() || sell_orders.empty())
    {
        return false;
    }

    return buy_orders.top().get_price() >= sell_orders.top().get_price();
}

size_t OrderBook::get_buy_orders_size() const
{
    return buy_orders.size();
}

size_t OrderBook::get_sell_orders_size() const
{
    return sell_orders.size();
}