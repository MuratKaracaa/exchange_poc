#include "stock.h"

OrderBook &Stock::get_order_book()
{
    return orderBook_;
}

void Stock::update_trading_info(double price, long quantity)
{
    TradingInfo current = tradingInfo.load();
    TradingInfo updated;

    do
    {
        updated.latestTradingPrice = price;
        updated.volume = current.volume + quantity;
    } while (!tradingInfo.compare_exchange_weak(current, updated));
}

TradingInfo Stock::get_trading_info() const
{
    return tradingInfo;
}

const std::string &Stock::get_symbol() const
{
    return symbol;
}

const std::string &Stock::get_stock_name() const
{
    return stockName;
}

void Stock::set_symbol(const std::string &sym)
{
    symbol = sym;
}

void Stock::set_stock_name(const std::string &name)
{
    stockName = name;
}