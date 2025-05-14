#pragma once

#include "Application.h"
#include <ankerl/unordered_dense.h>
#include <MessageCracker.h>
#include "stock.h"
#include "session_set.h"

class AcceptorApplication : public FIX::Application, public FIX::MessageCracker
{
private:
    ankerl::unordered_dense::map<std::string, Stock> &stock_map;
    moodycamel::ConcurrentQueue<Order> &order_queue;
    SessionSet &session_set;

public:
    AcceptorApplication(ankerl::unordered_dense::map<std::string, Stock> &stock_map_, moodycamel::ConcurrentQueue<Order> &order_queue_, SessionSet &session_set_);

    void onCreate(const FIX::SessionID &session_id) override;
    void onLogon(const FIX::SessionID &session_id) override;
    void onLogout(const FIX::SessionID &session_id) override;
    void toAdmin(FIX::Message &message, const FIX::SessionID &session_id) override;
    void toApp(FIX::Message &message, const FIX::SessionID &session_id) override;
    void fromAdmin(const FIX::Message &message, const FIX::SessionID &session_id) override;
    void fromApp(const FIX::Message &message, const FIX::SessionID &session_id) override;
    void onMessage(const FIX42::NewOrderSingle &order, const FIX::SessionID &session_id) override;
};
