#include <string>
#include <atomic>
#include "concurrentqueue.h"
#include "NewOrderSingle.h"

class Stock {
    std::string stockId;
    std::string stockName;
    double latestTradingPrice;
    long volume;
    moodycamel::ConcurrentQueue<FIX42::NewOrderSingle> buyingOrders;
    moodycamel::ConcurrentQueue<FIX42::NewOrderSingle> sellingOrders;
};