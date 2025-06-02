#pragma once

#include <string>
#include "SessionID.h"
#include "NewOrderSingle.h"

class Order
{
private:
  FIX::SessionID session_id_;
  std::string order_id_;
  std::string symbol_;
  FIX::OrdType orderType_;
  FIX::Side orderSide_;
  int quantity_;
  double price_;
  std::string timestamp_;

public:
  Order() : order_id_(""),
            symbol_(""),
            orderType_(FIX::OrdType_LIMIT),
            orderSide_(FIX::Side_BUY),
            quantity_(0),
            price_(0),
            timestamp_("") {}

  Order(FIX::SessionID session_id, std::string order_id, std::string symbol, FIX::OrdType orderType, FIX::Side orderSide, int quantity,
        double price, std::string timestamp = "")
      : session_id_(std::move(session_id)), order_id_(std::move(order_id)), symbol_(std::move(symbol)), orderType_(orderType), orderSide_(orderSide),
        quantity_(quantity), price_(price), timestamp_(timestamp) {}

  const FIX::SessionID &get_session_id() const;
  const std::string &get_order_id() const;
  const std::string &get_symbol() const;
  FIX::OrdType get_order_type() const;
  int get_quantity() const;
  double get_price() const;
  FIX::Side get_order_side() const;
  const std::string &get_time_stamp() const;
  void reduce_quantity(const int quantity);
};