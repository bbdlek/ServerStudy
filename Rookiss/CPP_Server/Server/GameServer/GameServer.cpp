#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)

		bool expected = false;
		bool desired = true;

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false; // compare_exchange_strong 에서 bool&를 참조하고 잇으므로 초기값으로 초기화 필요

			//this_thread::sleep_for(std::chrono::milliseconds(100));
			this_thread::sleep_for(100ms);
			//this_thread::yield(); // == this_thread::sleep_for(0ms);
		}
	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false); // atomic 사용중임을 알 수 있게
	}

private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
	
}