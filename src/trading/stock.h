#include <string>
#include <atomic>
#include "concurrentqueue.h"

class Stock {
    std::string stockId;
    std::string stockName;
    double latestTradingPrice;
    long volume;
    moodycamel::ConcurrentQueue<int> buyingOrders;
    moodycamel::ConcurrentQueue<int> sellingOrders;
};