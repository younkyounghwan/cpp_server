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
#include "Memory.h"


class Player
{
public:
	Player() {}
	virtual ~Player() {}
};

class Knight : public Player
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

	int32 _hp = 100;
	int32 _mp = 10;
};

int main()
{
	// 가상 메모리 기본

	//int* num = new int;
	//*num = 100;

	// 유저 레벨 (메모장, 롤, 서버)
	// ------------------
	// 커널레벨 (OS Code)

	//2GB [                            ]
	//2GB / 4KB [r][w][rw][x][][][][][][][] 
	// 페이지 단위로 설정을 해서 보안 레벨을 설정 할 수 있다.

	/*
	SYSTEM_INFO info;
	::GetSystemInfo(&info);

	info.dwPageSize; // 4KB (0x1000)
	info.dwAllocationGranularity; // 64KB (0x1000)

	Knight* k1 = new Knight();
	*/

	//int* test = new int; // (int*)::VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//*test = 100;
	//delete test;
	////::VirtualFree(test, 0, MEM_RELEASE);

	//*test = 200;


	//int64 address = reinterpret_cast<int64>(&num);
	//cout << address << endl;

	//
	//int* num2 = reinterpret_cast<int*>(12314314);
	//*num2 = 200;

	//delete num;



	/*
	knight* k1 = new knight();
	k1->_hp = 200;
	k1->_mp = 50;
	delete k1;
	k1 = nullptr;
	k1->_hp = 100; // use-after-free
	*/

	/*
	vector<int32> v{1,2,3,4,5};
	for (int32 i = 0; i < 5; i++)
	{
		int32 value = v[i];
		//TODO
		if (value == 3)
		{
			v.clear();
		}
	}
	*/

	/*
	Player* p = new Player();
	Knight* k = static_cast<Knight*>(p);

	k->_hp = 200;
	*/
	// [                   [          ] ]
	Knight* knight = static_cast<Knight*> (xnew <Player>());  // (Knight*)xnew<Player>();

	knight->_hp = 100; // Crash

	xdelete(knight);


}
