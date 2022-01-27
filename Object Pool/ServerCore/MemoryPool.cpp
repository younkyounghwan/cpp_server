#include "pch.h"
#include "MemoryPool.h"

/*------------------
	MemoryPool
--------------------*/

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);

}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{// NULL checking
		::_aligned_free(memory);
	}

}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	// pool에 메모리 반납
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_allocCount.fetch_sub(1);


}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));


	//없으면 새로 만든다.
	if (memory == nullptr)
	{
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return memory;
}
