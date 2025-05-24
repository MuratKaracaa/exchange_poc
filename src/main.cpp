#include <iostream>
#include <string>
#include <fstream>
#include <FileStore.h>
#include <FileLog.h>
#include <SocketAcceptor.h>
#include <ThreadedSocketAcceptor.h>
#include <Session.h>
#include <SessionSettings.h>
#include <Application.h>
#include <MessageCracker.h>
#include <NewOrderSingle.h>
#include "acceptor_application.h"
#include "order_consumer_pool.h"
#include "execution_publisher.h"
#include "constants.h"
#include "stock_loader.h"
#include <chrono>
#include <thread>

int main()
{
    ankerl::unordered_dense::map<std::string, Stock> stock_map;
    moodycamel::ConcurrentQueue<Order> order_queue;
    moodycamel::ConcurrentQueue<std::shared_ptr<NotificationBase>> notification_queue;
    SessionSet session_set;

    try
    {

        std::string stocks_file = "src/stocks.json";
        if (!loadStocksFromJSON(stocks_file, stock_map))
        {
            std::cerr << "Failed to load stocks from " << stocks_file << std::endl;
            return 1;
        }

        ExecutionPublisher execution_publisher(
            notification_queue,
            session_set,
            execution_publisher_thread_count,
            execution_publisher_batch_size);
        execution_publisher.start();

        global_processing_state.store(true, std::memory_order_relaxed);

        OrderConsumerPool order_consumer_pool(
            order_queue,
            stock_map,
            execution_publisher,
            processor_thread_count,
            processed_batch_size);
        order_consumer_pool.start();

        std::string configFile = "src/quickfix_config.cfg";
        FIX::SessionSettings settings(configFile);

        AcceptorApplication application(stock_map, order_queue, session_set);
        FIX::FileStoreFactory storeFactory(settings);
        FIX::FileLogFactory logFactory(settings);

        FIX::ThreadedSocketAcceptor acceptor(application, storeFactory, settings, logFactory);
        acceptor.start();

        std::cout << "QuickFIX acceptor started. Press Enter to quit." << std::endl;
        std::cin.get();

        std::cout << "Shutting down..." << std::endl;

        acceptor.stop();
        std::cout << "Acceptor stopped" << std::endl;

        global_processing_state.store(false, std::memory_order_relaxed);
        order_consumer_pool.stop();
        std::cout << "Order processing stopped" << std::endl;

        execution_publisher.stop();
        std::cout << "Execution publisher stopped" << std::endl;

        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
