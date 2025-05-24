#pragma once

#include <string>
#include <ankerl/unordered_dense.h>
#include "stock.h"

bool loadStocksFromJSON(const std::string &filename, ankerl::unordered_dense::map<std::string, Stock> &stock_map);