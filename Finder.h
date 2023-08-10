#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <filesystem>
#include <functional>

class ThreadPool
{
public:
	ThreadPool(int numThreads);
	~ThreadPool();

	template <typename Func>
	void enqueue(Func func);

	void stopThreads();
private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mutex;
	std::condition_variable condition;
	bool stop;

	void workerThread();


};

void searchFileInDirectory(ThreadPool& threadPool, const std::string& directory, const std::string& fileName);

template<typename Func>
inline void ThreadPool::enqueue(Func func)
{
	{
		std::unique_lock<std::mutex> lock(mutex);
		tasks.push(std::function<void()>(func));
	}
	condition.notify_one();
}