#pragma once

#include "Application.h"
#include <ankerl/unordered_dense.h>
#include <MessageCracker.h>
#include "stock.h"

class AcceptorApplication : public FIX::Application, public FIX::MessageCracker {
private:
    ankerl::unordered_dense::map<std::string, Stock> stock_map_;
    moodycamel::ConcurrentQueue<Order> order_queue_;

public:
    AcceptorApplication(ankerl::unordered_dense::map<std::string, Stock>& stock_map, moodycamel::ConcurrentQueue<Order>& order_queue);

    void onCreate(const FIX::SessionID& sessionID) override;
    void onLogon(const FIX::SessionID& sessionID) override;
    void onLogout(const FIX::SessionID& sessionID) override;
    void toAdmin(FIX::Message& message, const FIX::SessionID& sessionID) override;
    void toApp(FIX::Message& message, const FIX::SessionID& sessionID) override;
    void fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID) override;
    void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) override;
    void onMessage(const FIX42::NewOrderSingle& order, const FIX::SessionID& sessionId) override;
};
