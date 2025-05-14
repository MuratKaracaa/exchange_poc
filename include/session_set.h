#pragma once

#include <shared_mutex>
#include <unordered_set>
#include <SessionID.h>
#include <functional>
#include <ankerl/unordered_dense.h>

struct SessionIDHash
{
    std::size_t operator()(const FIX::SessionID &sessionID) const;
};

class SessionSet
{
public:
    SessionSet() = default;

    bool insert(const FIX::SessionID &sessionID);

    bool erase(const FIX::SessionID &sessionID);

    bool contains(const FIX::SessionID &sessionID) const;

    void forEach(const std::function<void(const FIX::SessionID &)> &func) const;

private:
    mutable std::shared_mutex mutex_;
    ankerl::unordered_dense::set<FIX::SessionID, SessionIDHash> sessions_;
};