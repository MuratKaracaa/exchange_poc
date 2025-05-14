#include "mapper.h"

std::optional<Order> Mapper::from_fix_to_order(const FIX42::NewOrderSingle &order, const std::string session_id)
{
    try
    {
        FIX::Symbol symbol;
        FIX::Side side;
        FIX::OrderQty quantity;
        FIX::OrdType ordType;
        FIX::ClOrdID order_id;

        order.get(symbol);
        order.get(side);
        order.get(quantity);
        order.get(ordType);
        order.get(order_id);

        double price = 0.0;
        if (ordType == FIX::OrdType_LIMIT)
        {
            FIX::Price fixPrice;
            order.get(fixPrice);
            price = fixPrice;
        }
        std::string timestamp;
        try
        {
            FIX::TransactTime transactTime;
            order.get(transactTime);
            timestamp = transactTime.getString();
        }
        catch (...)
        {
            FIX::SendingTime sendingTime;
            order.getHeader().get(sendingTime);
            timestamp = sendingTime.getString();
        }

        OrderType orderType = (ordType == FIX::OrdType_LIMIT) ? OrderType::LIMIT : OrderType::MARKET;
        OrderSide orderSide = side == FIX::Side_BUY ? OrderSide::BUY : OrderSide::SELL;

        return Order(session_id, order_id, symbol.getValue(), orderType, orderSide, static_cast<int>(quantity), price, timestamp);
    }
    catch (const std::exception &e)
    {
        return std::nullopt;
    }
}