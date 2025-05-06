#include "acceptor_application.h"
#include <iostream>
#include "NewOrderSingle.h"
#include "mapper.h"
#include <optional>

AcceptorApplication::AcceptorApplication(ankerl::unordered_dense::map<std::string, Stock>& stock_map)
    : stock_map(stock_map) {}


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
            stock_map[order.getSymbol()].incomingOrders.enqueue(std::move(order));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

