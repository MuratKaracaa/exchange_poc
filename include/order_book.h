#include <boost/heap/d_ary_heap.hpp>
#include "order.h"
#include "order_side.h"
#include "order_type.h"

constexpr int heap_arity = 4;
constexpr bool heap_mutability = false;

struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) const {

        if (a.getPrice() != b.getPrice()) {
            return a.getPrice() < b.getPrice();
        }
        return a.getTimestamp() > b.getTimestamp();
    }
};

struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.getPrice() != b.getPrice()) {
            return a.getPrice() > b.getPrice(); 
        }
        return a.getTimestamp() > b.getTimestamp();
    }
};

typedef boost::heap::d_ary_heap<
    Order, 
    boost::heap::arity<heap_arity>,
    boost::heap::compare<BuyOrderComparator>,
    boost::heap::mutable_<heap_mutability>
> BuyOrderQueue;

typedef boost::heap::d_ary_heap<
    Order, 
    boost::heap::arity<heap_arity>,
    boost::heap::compare<SellOrderComparator>,
    boost::heap::mutable_<heap_mutability>
> SellOrderQueue;

class OrderBook {
    private:
        BuyOrderQueue buyOrders;
        SellOrderQueue sellOrders;
    
    public:
        void addOrder(const Order& order) {
            if (order.getOrderSide() == OrderSide::BUY) {
                buyOrders.push(order);
            } else {
                sellOrders.push(order);
            }
        }
        
        std::optional<Order> getTopBuyOrder() {
            if (buyOrders.empty()) {
                return std::nullopt;
            }
            Order top = buyOrders.top();
            buyOrders.pop();
            return top;
        }
        
        std::optional<Order> getTopSellOrder() {
            if (sellOrders.empty()) {
                return std::nullopt;
            }
            Order top = sellOrders.top();
            sellOrders.pop();
            return top;
        }
        
        bool getCanMatch() const {
            if (buyOrders.empty() || sellOrders.empty()) {
                return false;
            }

            return buyOrders.top().getPrice() >= sellOrders.top().getPrice();
        }
        

        size_t getBuyOrdersSize() const { 
            return buyOrders.size(); 
        }

        size_t getSellOrdersSize() const { 
            return sellOrders.size(); 
        }
    };