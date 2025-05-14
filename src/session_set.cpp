#include "session_set.h"

std::size_t SessionIDHash::operator()(const FIX::SessionID &sessionID) const
{
    return std::hash<std::string>{}(sessionID.toStringFrozen());
}

bool SessionSet::insert(const FIX::SessionID &sessionID)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    return sessions_.insert(sessionID).second;
}

bool SessionSet::erase(const FIX::SessionID &sessionID)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    return sessions_.erase(sessionID) > 0;
}

bool SessionSet::contains(const FIX::SessionID &sessionID) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return sessions_.find(sessionID) != sessions_.end();
}

void SessionSet::forEach(const std::function<void(const FIX::SessionID &)> &func) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    for (const auto &sessionID : sessions_)
    {
        func(sessionID);
    }
}