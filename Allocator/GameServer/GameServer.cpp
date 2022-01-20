﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"


class Knight 
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp) 
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	//static void* operator new(size_t size)
	//{
	//	cout << "Knight new! " << size << endl;
	//	void* ptr = ::malloc(size);
	//	return ptr;
	//}

	//static void operator delete(void* ptr)
	//{
	//	cout << "Knight delete!" << endl;
	//	::free(ptr);
	//}

	int32 _hp = 100;
	int32 _mp = 10;
};

// new operator overloading (Global)

void* operator new(size_t size)
{
	cout << "new! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete(void* ptr)
{
	cout << "delete!" << endl;
	::free(ptr);
}

void* operator new[](size_t size)
{
	cout << "new! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete[](void* ptr)
{
	cout << "delete!" << endl;
	::free(ptr);
}



int main()
{
	Knight* knight = xnew<Knight>();

	xdelete(knight);

}
