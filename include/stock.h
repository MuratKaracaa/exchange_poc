#include <string>
#include <atomic>
#include "concurrentqueue.h"
#include "order.h"

class Stock {
    std::string symbol;
    std::string stockName;
    double latestTradingPrice;
    long volume;
public:
    moodycamel::ConcurrentQueue<Order> incomingOrders;
};