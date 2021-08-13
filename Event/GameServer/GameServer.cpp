#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue < int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);

		}
		SetEvent(handle);
		this_thread::sleep_for(100ms);

	}
}
void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE);
		// ::ResetEvent(handle);
		// Non-Signal    / ManualReset condition에 따라 다름
		

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// 커널 오브잭트
	// Usage Count
	//signal(파란불) / Non-Signal(빨간불) << bool
	// Auto / Manual << bool
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
	
	::CloseHandle(handle);

}
