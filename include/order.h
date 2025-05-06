// Update to Order class
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

    const std::string& getSymbol() const { 
        return symbol_; 
    }
    OrderType getOrderType() const { 
        return orderType_; 
    }
    int getQuantity() const { 
        return quantity_; 
    }
    double getPrice() const { 
        return price_; 
    }
    OrderSide getOrderSide() const {
        return orderSide_;
    }
    const std::string& getTimestamp() const {
        return timestamp_;
    }
};