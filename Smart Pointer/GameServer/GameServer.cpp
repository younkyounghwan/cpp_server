#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

#include "RefCounting.h"

using KnightRef = TSharedPtr<class Knight>;
using InventoryRef = TSharedPtr<class Inventory>;

class Knight //: public RefCountable
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	//void SetTarget(Knight target)
	//{
	//	_target = target;
	//}

	//Knight _target = nullptr;
	//InventoryRef _inventory = nullptr;
};

//class Inventory : public RefCountable
//{
//public:
//	Inventory(KnightRef knight) : _knight(**knight)
//	{}
//
//
//	Knight& _knight;
//	//KnightRef _knight; Cycle issue
//};

int main()
{
	// 1) 이미 만들어진 클래스 대상으로 사용 불가
	// 2) 순환 (Cycle) 문제

	//KnightRef k1(new Knight());
	//k1->ReleaseRef();

	//unique_ptr<Knight> k2 = make_unique<Knight>();
	//// unique_ptr<Knight> k3 = k2; // 생명주기에 있어서 복사 불가능
	//unique_ptr<Knight> k3 = std::move(k2);

	// unique_ptr
	
	// shared_ptr
	// weak_ptr
	
	// [Knight][RefCountingBlock] // new Knight
	// [Knight | RefCountingBlock] // make_shared

	// [Knight | RefCountingBlock(uses, weak)]

	// [T*][RefCountBlock*]

	// RefCountBlock(useCount(shared), weakCount)
	shared_ptr<Knight> spr = make_shared<Knight>();
	weak_ptr<Knight> wpr = spr;

	bool expired = wpr.expired();
	shared_ptr<Knight> spr2 = wpr.lock();

	// [T*][RefCountBlock*]
	//shared_ptr<Knight> spr2 = spr;
	//if (spr2 != nullptr) {} // weak_ptr을 shared_ptr로 전환하면 다음과 같이 메모리 확인을 한다.

}
