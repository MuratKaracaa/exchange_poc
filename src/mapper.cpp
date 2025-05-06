#include "mapper.h"

std::optional<Order> Mapper::fromFixToOrder(const FIX42::NewOrderSingle& order) {
    try {
        FIX::Symbol symbol;
        FIX::Side side;
        FIX::OrderQty quantity;
        FIX::OrdType ordType;
        
        order.get(symbol);
        order.get(side);
        order.get(quantity);
        order.get(ordType);
        
        if(quantity <= 0){
            return std::nullopt;
        }
        
        double price = 0.0;
        if (ordType == FIX::OrdType_LIMIT) {
            FIX::Price fixPrice;
            order.get(fixPrice);
            if(fixPrice <= 0){
                return std::nullopt;
            }
            price = fixPrice;
        }
        
        std::string timestamp;
        try {
            FIX::TransactTime transactTime;
            order.get(transactTime);
            timestamp = transactTime.getString();
        } catch (...) {
            FIX::SendingTime sendingTime;
            order.getHeader().get(sendingTime);
            timestamp = sendingTime.getString();
        }
        
        OrderType orderType = (ordType == FIX::OrdType_LIMIT) ? OrderType::LIMIT : OrderType::MARKET;
        OrderSide orderSide = side == FIX::Side_BUY ? OrderSide::BUY : OrderSide::SELL;
        
        return Order(symbol.getValue(), orderType, orderSide, static_cast<int>(quantity), price, timestamp);
    } catch (const std::exception& e) {
        return std::nullopt;
    }
}