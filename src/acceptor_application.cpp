#include "acceptor_application.h"
#include <iostream>
#include "NewOrderSingle.h"
#include "mapper.h"
#include <optional>

AcceptorApplication::AcceptorApplication(ankerl::unordered_dense::map<std::string, Stock>& stock_map, moodycamel::ConcurrentQueue<Order>& order_queue)
    : stock_map_(stock_map), order_queue_(order_queue) {}


void AcceptorApplication::onCreate(const FIX::SessionID &sessionID)
{
    std::cout << "Session created: " << sessionID << std::endl;
}

void AcceptorApplication::onLogon(const FIX::SessionID& sessionID) {
    std::cout << "Logon: " << sessionID << std::endl;
}

void AcceptorApplication::onLogout(const FIX::SessionID& sessionID) {
    std::cout << "Logout: " << sessionID << std::endl;
}

void AcceptorApplication::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) {
    crack(message, sessionID);
}

void AcceptorApplication::toAdmin(FIX::Message& message, const FIX::SessionID& sessionID) {

}
void AcceptorApplication::toApp(FIX::Message& message, const FIX::SessionID& sessionID) {

}

void AcceptorApplication::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID) {
    
}

void AcceptorApplication::onMessage(const FIX42::NewOrderSingle& fixOrder, const FIX::SessionID& sessionId) {
    try
    {
        std::optional<Order> optionalOrder = Mapper::fromFixToOrder(fixOrder);
        if (optionalOrder) {
            Order order = std::move(*optionalOrder);
            if(!stock_map_.contains(order.getSymbol())){

            }

            if(order.getPrice() <= 0) {

            }
            if(order.getQuantity() <= 0){

            }
            order_queue_.enqueue(order);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

