#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	//account Lock
	Account* account = AccountManager::instance()->getAccount(100);

	// userLock
	lock_guard<mutex> guard(_mutex);

}