#include <boost/heap/d_ary_heap.hpp>
#include "order.h"
#include "order_side.h"
#include "order_type.h"
#include "constants.h"

struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) const {

        if (a.get_price() != b.get_price()) {
            return a.get_price() < b.get_price();
        }
        return a.get_time_stamp() > b.get_time_stamp();
    }
};

struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.get_price() != b.get_price()) {
            return a.get_price() > b.get_price(); 
        }
        return a.get_time_stamp() > b.get_time_stamp();
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
        BuyOrderQueue buy_orders;
        SellOrderQueue sell_orders;
    
    public:
        void addOrder(const Order& order) {
            if (order.get_order_side() == OrderSide::BUY) {
                buy_orders.push(order);
            } else {
                sell_orders.push(order);
            }
        }
        
        std::optional<Order> get_top_buy_order() {
            if (buy_orders.empty()) {
                return std::nullopt;
            }
            Order top = buy_orders.top();
            buy_orders.pop();
            return top;
        }
        
        std::optional<Order> get_top_sell_order() {
            if (sell_orders.empty()) {
                return std::nullopt;
            }
            Order top = sell_orders.top();
            sell_orders.pop();
            return top;
        }
        
        bool get_can_match() const {
            if (buy_orders.empty() || sell_orders.empty()) {
                return false;
            }

            return buy_orders.top().get_price() >= sell_orders.top().get_price();
        }
        

        size_t get_buy_orders_size() const { 
            return buy_orders.size(); 
        }

        size_t get_sell_orders_size() const { 
            return sell_orders.size(); 
        }
    };