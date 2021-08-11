#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		//CAS (Compare-And-Swap)

		bool expected = false;
		bool desired = true;

		// CAS 의사 코드

		/*
		if (_locked == expected)
		{
			expected = _locked;
			_locked = desired;
			return true;
		}

		else
		{
			expected = _locked;
			return false;
		}
		*/

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}

		/*while (_locked)
		{

		}

		_locked = true;*/

	}
	
	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false; 
	// c++에서 volatile은 최적화를 하지 말아달라는 것에 불과

};

int32 sum = 0;
mutex m;
SpinLock spinLock;


void Add()
{
	for (int32 i = 0; i < 100'000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}
