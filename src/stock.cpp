#include "stock.h"

OrderBook &Stock::get_order_book()
{
    return orderBook_;
}

void Stock::update_volume(long quantity)
{
    volume += quantity;
}

long Stock::get_volume() const
{
    return volume;
}