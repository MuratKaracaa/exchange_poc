#include "stock.h"

OrderBook &Stock::get_order_book()
{
    return orderBook_;
}

void Stock::update_trading_info(double price, long quantity)
{
    TradingInfo current = tradingInfo.load();
    TradingInfo updated(price, current.volume + quantity);

    while (!tradingInfo.compare_exchange_weak(current, updated))
    {
        updated = TradingInfo(price, current.volume + quantity);
    }
}

TradingInfo Stock::get_trading_info() const
{
    return tradingInfo;
}