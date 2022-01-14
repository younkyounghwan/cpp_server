#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

class TestLock
{
	USE_LOCK;
	// Lock _locks[1];

public:
	int32 TestRead()
	{
		READ_LOCK;

		if (_queue.empty())
			return -1;
		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		WRITE_LOCK;

		if (_queue.empty() == false)
			_queue.pop();
	}

private:
	queue<int32> _queue;

};

TestLock testLock;


void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}


mutex m; // 표준 mutex에서는 재귀적으로 lock을 잡을 수 없다.


// 99.99999999999999999 % 일정
//  0.00000000000000001 % 변경 있을 수 있음
// 이런 경우 lock을 사용하기에 조금 애매함.

list<int32> GetRewardList(int32 questId)
{
	m.lock();

	list<int32> ret;

	return ret;

}

void ThreadMain()
{
	while (true)
	{
		cout << "Hello! I am thread... " << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadWrite);

	}

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadRead);

	}

	GThreadManager->Join();


}