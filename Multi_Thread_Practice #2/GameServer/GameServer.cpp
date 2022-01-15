#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

#include <thread>
#include <vector>
#include <algorithm>
#include <queue>

#include <cstdio>
#include <iostream>

using namespace std;
int main()
{
	int n1, n2;
	vector<bool> vec1; //양수 0
	vector<bool> vec2; //음수

	scanf_s("%d",&n1);

	for (int i = 0; i < 1'000'001; i++)
	{
		vec1.push_back(false);
	} // vec[0] vec[1'000'000]

	for (int i = 0; i < 1'000'000; i++)
	{
		vec2.push_back(false);
	} // vec[0] vec[1'000'000-1]

	for (int i = 0; i < n1; i++)
	{	
		scanf_s("%d",&n2);
		if (n2 >= 0)
			vec1[n2] = true;
		else
			vec2[-n2] = true;
	}
	
	for (int i = 1'000'000-1; i >= 0; i--)
	{
		if (vec2[i+1] == true)
			printf("%d\n", -(i+1));
	}

	for (int i = 0; i < 1000001; i++)
	{
		if (vec1[i] == true)
			printf("%d\n", i);
	}
}

