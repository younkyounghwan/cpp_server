#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

thread_local int32 LThreadId = 0;
// thread_local queue<int32> q;

void ThreadMain(int32 threadid)
{
	LThreadId = threadid;

	while (true)
	{
		cout << "Hi! I am Thread " << LThreadId << endl;
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadid = i + 1;
		threads.push_back(thread(ThreadMain, threadid));
	}

	for (thread& t : threads)
		t.join();
}