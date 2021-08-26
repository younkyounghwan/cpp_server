#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;
	ready.store(true, memory_order::memory_order_release);
	// std::atomic_thread_fence(memory_order::memory_order_release);
}

void Consumer()
{
	while (ready.load(memory_order::memory_order_release));
	cout << value << endl;
}

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

	// Memory Model(정책)
	// 1) Sequenctially Consistance (seq_cst)
	// 2) Acquire_Release (acquire, release)
	// 3) Relaxed (relaxed)
	// 1) seq_cst  (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	// 가시성 문제 바로 해결, 코드 재배치 문제 해결

	// 2) acquire_release
	// 딱 중간
	// release 명령 이전의 메모리 명령들이, 해당 명령 이후로 재배치 되는 것을 금지
	// 그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면,
	// release 이전의 명령들이 -> acquire 하는 순간에 관찰 가능 (가시성 보장)

	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 너무나도 자유롭다!
	// 코드 재배치도 멋대로 가시성 해결 No!
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)
	// 인텔, amd의 경우 애당초 순차적 일관성을 보장해서
	// seq_cst를 써도 별다른 부하가 없다.

	cout << endl;

	atomic<bool> flag = false;
	flag.is_lock_free(); // ~~이 없다
	// flag = true;
	flag.store(true, memory_order::memory_order_seq_cst);
	// bool val = flag;
	bool val = flag.load(memory_order::memory_order_seq_cst);
	// 이전 flag값을 prev에 넣고, flag 값을 수정
	{
		bool prev = flag.exchange(true);
		// bool prev = flag;
		// flag = true;
	}

	// CAS (Compare-And-Swap) 조건부 수정
	{
		bool expected = false;
		bool desired = true;
		flag.compare_exchange_strong(expected, desired);


		// Spurios Failure
		if (flag == expected)
		{
			//expected = flag;
			flag = desired;
			return true;
		}

		else
		{
			expected = flag;
			return false;
		}

		cout << endl;

		while (true)
		{
			bool expected = false;
			bool desired = true;
			flag.compare_exchange_weak(expected, desired);
		}
	}
}