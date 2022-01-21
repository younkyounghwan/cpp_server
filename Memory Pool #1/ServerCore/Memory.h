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
		// ~1024byte������ 32byte����, ~2048byte���� 128byte����, ~4096byte���� 256byte����
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

	// �޸� ũ�� <-> �޸� Ǯ
	// 0(1) ������ ã�� ���� ���̺�
	// 0~32byte
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args> // ������ ������ ���������� ����
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