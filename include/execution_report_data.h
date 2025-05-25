#pragma once

#include <string>
#include <memory>
#include "SessionID.h"
#include "order_side.h"
#include "order_type.h"
#include "notification_type.h"

struct NotificationBase
{
    NotificationType type;
    std::string symbol;

    NotificationBase(NotificationType type_, const std::string &symbol_)
        : type(type_), symbol(symbol_) {}

    virtual ~NotificationBase() = default;
};

struct ExecutionReportData : public NotificationBase
{
    FIX::SessionID client_session_id;
    std::string order_id;
    int executed_quantity;
    double execution_price;
    std::string execution_time;

    ExecutionReportData(
        const std::string &symbol_,
        const FIX::SessionID &client_session_id_,
        const std::string &order_id_,
        int executed_qty_,
        double execution_price_,
        const std::string &execution_time_ = "") : NotificationBase(NotificationType::EXECUTION_REPORT, symbol_),
                                                   client_session_id(client_session_id_),
                                                   order_id(order_id_),
                                                   executed_quantity(executed_qty_),
                                                   execution_price(execution_price_),
                                                   execution_time(execution_time_) {}
};

struct MarketDataUpdate : public NotificationBase
{
    double last_trade_price;
    int last_trade_quantity;
    long total_volume;
    std::string trade_time;

    MarketDataUpdate(
        const std::string &symbol_,
        double last_trade_price_,
        int last_trade_quantity_,
        long total_volume_,
        const std::string &trade_time_ = "") : NotificationBase(NotificationType::MARKET_DATA_UPDATE, symbol_),
                                               last_trade_price(last_trade_price_),
                                               last_trade_quantity(last_trade_quantity_),
                                               total_volume(total_volume_),
                                               trade_time(trade_time_) {}
};