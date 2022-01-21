#pragma once
#include "Allocator.h"

//template<typename Type>
//Type* xnew()
//{
//	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type));
//
//	//placement new
//	new(memory) Type();
//
//	return memory;
//}
//
//template<typename Type, typename T1>
//Type* xnew(T1 t1)
//{
//	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type));
//
//	//placement new
//	new(memory) Type(t1);
//
//	return memory;
//}

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