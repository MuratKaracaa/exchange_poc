#include "acceptor_application.h"
#include <iostream>
#include "NewOrderSingle.h"

AcceptorApplication::AcceptorApplication(ankerl::unordered_dense::map<std::string, std::shared_ptr<Stock>>& stock_map)
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

void AcceptorApplication::onMessage(const FIX42::NewOrderSingle& order, const FIX::SessionID& sessionId) {

}
