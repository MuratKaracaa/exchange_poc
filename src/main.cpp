#include <iostream>
#include <string>
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
#include <atomic>

constexpr std::atomic_flag global_processing_state {false};


int main() {
    ankerl::unordered_dense::map<std::string, Stock> stock_map;
    moodycamel::ConcurrentQueue<Order> order_queue;
    try {
        std::string configFile = "quickfix_config.cfg";
        FIX::SessionSettings settings(configFile);
        
        AcceptorApplication application(stock_map, order_queue);
        FIX::FileStoreFactory storeFactory(settings);
        FIX::FileLogFactory logFactory(settings);
        
        FIX::ThreadedSocketAcceptor acceptor(application, storeFactory, settings, logFactory);
        acceptor.start();
        
        std::cout << "QuickFIX acceptor started. Press Enter to quit." << std::endl;
        std::cin.get();
        
        acceptor.stop();
        return 0;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
