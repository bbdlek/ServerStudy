#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		::SetEvent(handle);

		this_thread::sleep_for(1000000ms);
	}
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE);
		//::ResetEvent(handle); --> CreateEvent시 bManualReset : TRUE로 설정 할 시 호출할 것

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}


int main()
{
	// 커널 오브젝트 (커널에서 관리하는 오브젝트)
	// Usage Count
	// Signal (파란불) / Non-Signal (빨간불) << bool
	// Auto / Manual << bool
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
	
	::CloseHandle(handle);
}