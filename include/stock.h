#pragma once

#include "order_book.h"
#include "concurrentqueue.h"

struct TradingInfo
{
    double latestTradingPrice;
    long volume;

    TradingInfo() = default;
    TradingInfo(double price, long vol) : latestTradingPrice(price), volume(vol) {}
};
static_assert(std::is_trivially_copyable_v<TradingInfo>);

class Stock
{
private:
    std::string symbol;
    std::string stockName;
    OrderBook orderBook_;
    std::atomic<TradingInfo> tradingInfo{TradingInfo{0.0, 0}};

public:
    Stock() = default;

    Stock(Stock &&other) noexcept
        : symbol(std::move(other.symbol)), stockName(std::move(other.stockName)), orderBook_(std::move(other.orderBook_)), tradingInfo(other.tradingInfo.load())
    {
    }

    Stock &operator=(Stock &&other) noexcept
    {
        if (this != &other)
        {
            symbol = std::move(other.symbol);
            stockName = std::move(other.stockName);
            orderBook_ = std::move(other.orderBook_);
            tradingInfo.store(other.tradingInfo.load());
        }
        return *this;
    }

    Stock(const Stock &) = delete;
    Stock &operator=(const Stock &) = delete;

    const std::string &get_symbol() const;
    const std::string &get_stock_name() const;

    void set_symbol(const std::string &sym);
    void set_stock_name(const std::string &name);

    OrderBook &get_order_book();
    TradingInfo get_trading_info() const;
    void update_trading_info(double price, long quantity);
};