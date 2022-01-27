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
#include "Allocator.h"


DECLSPEC_ALIGN(16)
class Data // : public SListEntry 
{
public:
	SLIST_ENTRY _entry;

	int64 _rand = rand() % 1000;
};
SLIST_HEADER* GHeader;
int main()
{


	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{

				}
			});
	}
	GThreadManager->Join();

}
