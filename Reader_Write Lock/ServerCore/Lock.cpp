#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	// 재귀적으로 동일한 쓰레드가 쇼유하고 있다면 무조건 성공.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을 때,
	// 경합해서 쇼유권을 얻는다.
	const int64 beginTick = GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++; // 재귀적으로 lock을 잡을 때 crash가 나는 것이
							   // 아닌 lock을 잡을 수 있도록 추적하는 변수
				return;
			}
		}
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	//ReadLock 다 출기 전에는 WriteUnlock 불가능
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
	{
		CRASH("INVAILD_UNLOCK_ORDER");
	}
	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.

	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
			{
				return;
			}
		}
		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");	
	}
}
