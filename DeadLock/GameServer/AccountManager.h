#pragma once
#include <mutex>

class Account
{
    //Todo
};

class AccountManager
{
public:
    static AccountManager* instance()
    {
        static AccountManager instance;
        return &instance;
    }

    Account* getAccount(int32 id)
    {
        lock_guard<mutex> guard(_mutex);
        //¹º°¡¸¦ °¡Áö°í ¿È
        return nullptr;
    }

    void ProcessLogin();

private:
    mutex _mutex;
    //map<int32, Account*> _accounts;

};