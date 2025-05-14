#include "acceptor_application.h"
#include <iostream>
#include "NewOrderSingle.h"
#include "mapper.h"
#include <optional>

AcceptorApplication::AcceptorApplication(ankerl::unordered_dense::map<std::string, Stock> &stock_map_, moodycamel::ConcurrentQueue<Order> &order_queue_, SessionSet &session_set_)
    : stock_map(stock_map_), order_queue(order_queue_), session_set(session_set_) {}

void AcceptorApplication::onCreate(const FIX::SessionID &sessionID)
{
    std::cout << "Session created: " << sessionID << std::endl;
}

void AcceptorApplication::onLogon(const FIX::SessionID &sessionID)
{
    session_set.insert(sessionID);
}

void AcceptorApplication::onLogout(const FIX::SessionID &sessionID)
{
    session_set.erase(sessionID);
}

void AcceptorApplication::fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
{
    crack(message, sessionID);
}

void AcceptorApplication::toAdmin(FIX::Message &message, const FIX::SessionID &sessionID)
{
}
void AcceptorApplication::toApp(FIX::Message &message, const FIX::SessionID &sessionID)
{
}

void AcceptorApplication::fromAdmin(const FIX::Message &message, const FIX::SessionID &sessionID)
{
}

void AcceptorApplication::onMessage(const FIX42::NewOrderSingle &fixOrder, const FIX::SessionID &sessionId)
{
    try
    {
        std::optional<Order> optionalOrder = Mapper::from_fix_to_order(fixOrder, sessionId);
        if (optionalOrder)
        {
            Order order = std::move(*optionalOrder);
            if (!stock_map.contains(order.get_symbol()))
            {
            }
            if (order.get_price() <= 0)
            {
            }
            if (order.get_quantity() <= 0)
            {
            }
            order_queue.enqueue(order);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
