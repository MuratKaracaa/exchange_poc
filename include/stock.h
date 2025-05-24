#pragma once

#include "order_book.h"
#include "concurrentqueue.h"

struct TradingInfo
{
    double latestTradingPrice;
    long volume;
};

class Stock
{
private:
    std::string symbol;
    std::string stockName;
    OrderBook orderBook_;
    std::atomic<TradingInfo> tradingInfo;

public:
    Stock() = default;

    OrderBook &get_order_book();
    TradingInfo get_trading_info() const;
    void update_trading_info(double price, long quantity);
};