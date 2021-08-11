#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for(int32 i = 0; i < 100; i++)
	{
		UserManager::instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 100; i++)
	{
		AccountManager::instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	//참고
	mutex m1;
	mutex m2;

	std::lock(m1, m2);

	// adopt_lock : 이미 lock된 상태니까, 나중에 소멸될 떄 풀어주기만 해
	lock_guard<mutex> g1(m1, std::adopt_lock);
	lock_guard<mutex> g2(m2, std::adopt_lock);
}