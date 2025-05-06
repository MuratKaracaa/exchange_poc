#include <string>
#include <atomic>
#include "concurrentqueue.h"
#include "order.h"
#include "order_book.h"

class Stock {
    std::string symbol;
    std::string stockName;
    double latestTradingPrice;
    long volume;
    OrderBook orderBook_;
public:
    moodycamel::ConcurrentQueue<Order> incomingOrders;
};