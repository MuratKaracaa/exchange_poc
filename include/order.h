#pragma once

#include <string>
#include "order_type.h"
#include "order_side.h"

class Order {

private:
    std::string symbol_;
    OrderType orderType_;
    OrderSide orderSide_;
    int quantity_;
    double price_;
    std::string timestamp_;

public:
    Order(std::string symbol, OrderType orderType, OrderSide orderSide, int quantity, 
          double price, std::string timestamp = "")
        : symbol_(std::move(symbol)), orderType_(orderType), orderSide_(orderSide),
          quantity_(quantity), price_(price), timestamp_(timestamp) {}

    const std::string& get_symbol() const { 
        return symbol_; 
    }
    OrderType get_order_type() const { 
        return orderType_; 
    }
    int get_quantity() const { 
        return quantity_; 
    }
    double get_price() const { 
        return price_; 
    }
    OrderSide get_order_side() const {
        return orderSide_;
    }
    const std::string& get_time_stamp() const {
        return timestamp_;
    }
};