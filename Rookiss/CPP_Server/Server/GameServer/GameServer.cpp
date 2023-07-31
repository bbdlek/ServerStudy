#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

// [1][2][3] -> delete가 중복으로 실행되면서 삭제 못함
// [1][2][3][][][][][][][][][] -> reserve 했어도 같은 곳에 push_back 할 수도 있음

vector<int32> v;

// Mutual Exclusive (상호배타적)
mutex m; // 일종의 화장실 자물쇠

// RAII (Resource Acquisition Is Initialization)
// 생성자에서 잠구고 소멸자에서 풀기
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard()
	{
		_mutex->unlock();
	}

private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; i++)
	{
		// 자물쇠 잠구기
		//LockGuard<std::mutex> lockGuard(m);
		std::lock_guard<std::mutex> lockGuard(m);
		//std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock); // 나중에 잠구기 가능

		//uniqueLock.lock();
		//m.lock();
		//m.lock();

		v.push_back(i);

		if (i == 5000)
		{
			//m.unlock();
			break;
		}

		// 자물쇠 풀기
		//m.unlock();
		//m.unlock();
	}
}

int main()
{
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;

}