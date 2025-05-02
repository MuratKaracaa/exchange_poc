#pragma once

#include "Application.h"
#include <ankerl/unordered_dense.h>
#include <MessageCracker.h>
#include "stock.h"

class AcceptorApplication : public FIX::Application, public FIX::MessageCracker {
private:
    ankerl::unordered_dense::map<std::string, std::shared_ptr<Stock>> stock_map;

public:
    AcceptorApplication(ankerl::unordered_dense::map<std::string, std::shared_ptr<Stock>>& stock_map);

    void onCreate(const FIX::SessionID& sessionID) override;
    void onLogon(const FIX::SessionID& sessionID) override;
    void onLogout(const FIX::SessionID& sessionID) override;
    void toAdmin(FIX::Message& message, const FIX::SessionID& sessionID) override;
    void toApp(FIX::Message& message, const FIX::SessionID& sessionID) override;
    void fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID) override;
    void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) override;
    void onMessage(const FIX42::NewOrderSingle& order, const FIX::SessionID& sessionId) override;
};
