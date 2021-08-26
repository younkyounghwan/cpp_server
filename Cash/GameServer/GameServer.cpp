#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

int buffer[10000][10000];

int main()
{
	memset(buffer, 0, sizeof(buffer));

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10000; i++)
			for (int32 j = 0; j < 10000; j++)
				sum += buffer[i][j];

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
		cout << start << " " << end << endl;
	}


	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10000; i++)
			for (int32 j = 0; j < 10000; j++)
				sum += buffer[j][i];

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
		cout << start << " " << end << endl;
	}
}
