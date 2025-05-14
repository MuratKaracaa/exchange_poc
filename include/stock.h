#pragma once

#include "order_book.h"
#include "concurrentqueue.h"

class Stock
{
private:
    std::string symbol;
    std::string stockName;
    double latestTradingPrice;
    long volume;
    OrderBook orderBook_;

public:
    Stock() = default;

    OrderBook &get_order_book()
    {
        return orderBook_;
    }
};