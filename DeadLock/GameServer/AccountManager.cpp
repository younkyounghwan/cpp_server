#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	//accountLock
	lock_guard<mutex> guard(_mutex);

	//userLock
	User* user = UserManager::instance()->GetUser(100);
}