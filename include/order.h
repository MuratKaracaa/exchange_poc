#pragma once

#include <string>
#include "order_type.h"
#include "order_side.h"
#include "SessionID.h"

class Order
{
private:
    FIX::SessionID session_id_;
    std::string order_id_;
    std::string symbol_;
    OrderType orderType_;
    OrderSide orderSide_;
    int quantity_;
    double price_;
    std::string timestamp_;

public:
    Order() : order_id_(""),
              symbol_(""),
              orderType_(OrderType::LIMIT),
              orderSide_(OrderSide::BUY),
              quantity_(0),
              price_(0),
              timestamp_("") {}

    Order(FIX::SessionID session_id, std::string order_id, std::string symbol, OrderType orderType, OrderSide orderSide, int quantity,
          double price, std::string timestamp = "")
        : session_id_(std::move(session_id)), order_id_(std::move(order_id)), symbol_(std::move(symbol)), orderType_(orderType), orderSide_(orderSide),
          quantity_(quantity), price_(price), timestamp_(timestamp) {}

    const FIX::SessionID &get_session_id() const
    {
        return session_id_;
    }

    const std::string &get_order_id() const
    {
        return order_id_;
    }

    const std::string &get_symbol() const
    {
        return symbol_;
    }
    OrderType get_order_type() const
    {
        return orderType_;
    }
    int get_quantity() const
    {
        return quantity_;
    }
    double get_price() const
    {
        return price_;
    }
    OrderSide get_order_side() const
    {
        return orderSide_;
    }
    const std::string &get_time_stamp() const
    {
        return timestamp_;
    }
    void reduce_quantity(const int quantity)
    {
        quantity_ -= quantity;
    }
};