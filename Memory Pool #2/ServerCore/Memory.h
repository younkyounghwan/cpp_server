#pragma once
#include "Allocator.h"

class MemoryPool;

/*----------
   Memory
----------*/

class Memory
{
	enum
	{
		// ~1024byte까지는 32byte단위, ~2048byte까지 128byte단위, ~4096byte까지 256byte단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void  Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// 0(1) 빠르게 찾기 위한 테이블
	// 0~32byte
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args> // 인자의 개수가 가변적으로 변함
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xxalloc(sizeof(Type)));

	new(memory) Type(forward<Args>(args)...);	//placement new

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xxrelease(obj);
}