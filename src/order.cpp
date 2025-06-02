#include "order.h"

const FIX::SessionID &Order::get_session_id() const
{
    return session_id_;
}

const std::string &Order::get_order_id() const
{
    return order_id_;
}

const std::string &Order::get_symbol() const
{
    return symbol_;
}

FIX::OrdType Order::get_order_type() const
{
    return orderType_;
}

int Order::get_quantity() const
{
    return quantity_;
}

double Order::get_price() const
{
    return price_;
}

FIX::Side Order::get_order_side() const
{
    return orderSide_;
}

const std::string &Order::get_time_stamp() const
{
    return timestamp_;
}

void Order::reduce_quantity(const int quantity)
{
    quantity_ -= quantity;
}