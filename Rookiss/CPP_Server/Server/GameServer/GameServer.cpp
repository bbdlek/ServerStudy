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

// 참고) CV는 User-Level Object (커널 오브젝트X)
condition_variable cv;

//#include <condition_variable>
//condition_variable_any cv;

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수 통해 다른 쓰레드에게 통지
		{
			unique_lock<mutex> lock(m); // 1)
			q.push(100); // 2)
		} // 3)

		cv.notify_one(); // 4) wait중인 쓰레드가 있으면 딱 1개를 깨운다

		//this_thread::sleep_for(1000000ms);
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m); // unique_lock을 사용하는 이유는 wait에서 풀어줄 수 있는 기능이 있어야하기 때문
		cv.wait(lock, []() { return q.empty() == false; }); // {}문 안의 조건이 만족할 때까지 기다림
		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족O => 빠져 나와서 이어서 코드를 진행
		// - 만족X => Lock을 풀어주고 대기 상태

		// 그런데 notify_one을 했으면 항상 조건식을 만족하는거 아닐까?
		// Spurious Wakeup (가짜 기상?)
		// notify_one 할 때 lock을 잡고 있는 것이 아니기 때문

		// if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
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