#pragma once
#include <mutex>

class Account
{
	// TODO
};


class AccountManager
{
public:
	static AccountManager* Instance() // Singleton
	{
		static AccountManager instance;
		return &instance;
	}

	Account* GetAccount(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		// ¹º°¡¸¦ °®°í ¿È
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;
	//map<int32, Account*> _accounts;

};
