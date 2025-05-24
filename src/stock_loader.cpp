#include "stock_loader.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool loadStocksFromJSON(const std::string &filename, ankerl::unordered_dense::map<std::string, Stock> &stock_map)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open " << filename << std::endl;
        return false;
    }

    try
    {
        // Parse the entire JSON file
        json stock_data = json::parse(file);

        if (!stock_data.contains("stocks") || !stock_data["stocks"].is_array())
        {
            std::cerr << "Invalid JSON format: missing or invalid 'stocks' array" << std::endl;
            return false;
        }

        int loaded_count = 0;
        for (const auto &stock_json : stock_data["stocks"])
        {
            // Extract stock data
            if (!stock_json.contains("symbol") || !stock_json["symbol"].is_string())
            {
                std::cerr << "Skipping stock with missing or invalid symbol" << std::endl;
                continue;
            }

            std::string symbol = stock_json["symbol"];

            // Create a new stock in the map with the symbol
            auto &stock = stock_map[symbol];

            // Set trading info if available
            if (stock_json.contains("tradingInfo"))
            {
                const auto &trading_info = stock_json["tradingInfo"];
                double price = 0.0;
                long volume = 0;

                if (trading_info.contains("latestTradingPrice") && trading_info["latestTradingPrice"].is_number())
                {
                    price = trading_info["latestTradingPrice"];
                }

                if (trading_info.contains("volume") && trading_info["volume"].is_number())
                {
                    volume = trading_info["volume"];
                }

                stock.set_stock_name(stock_json["stockName"]);
                stock.set_symbol(symbol);
                stock.update_trading_info(price, volume);
            }

            loaded_count++;
        }

        std::cout << "Loaded " << loaded_count << " stocks from " << filename << std::endl;
        return loaded_count > 0;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading stocks from JSON: " << e.what() << std::endl;
        return false;
    }
}