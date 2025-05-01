#include <iostream>
#include <string>
#include <quickfix/FileStore.h>
#include <quickfix/FileLog.h>
#include <quickfix/SocketAcceptor.h>
#include <quickfix/Session.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

class TestApplication : public FIX::Application, public FIX::MessageCracker {
public:
    void onCreate(const FIX::SessionID&) override {}
    void onLogon(const FIX::SessionID& sessionID) override {
        std::cout << "Logon - " << sessionID.toString() << std::endl;
    }
    void onLogout(const FIX::SessionID& sessionID) override {
        std::cout << "Logout - " << sessionID.toString() << std::endl;
    }
    void toAdmin(FIX::Message&, const FIX::SessionID&) override {}
    void fromAdmin(const FIX::Message&, const FIX::SessionID&)  override {}
    void toApp(FIX::Message&, const FIX::SessionID&)  override {}
    void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)  override {
        crack(message, sessionID);
        std::cout << "Message received: " << message.toString() << std::endl;
    }
};

int main() {
    try {
        std::string configFile = "quickfix_config.cfg";
        FIX::SessionSettings settings(configFile);
        
        TestApplication application;
        FIX::FileStoreFactory storeFactory(settings);
        FIX::FileLogFactory logFactory(settings);
        
        FIX::SocketAcceptor acceptor(application, storeFactory, settings, logFactory);
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