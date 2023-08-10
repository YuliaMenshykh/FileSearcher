
#include <iostream>
#include "Finder.h"

ThreadPool::ThreadPool(int numThreads)
    :stop (false)
{
    for (int i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]() { workerThread(); });
    }
}

ThreadPool::~ThreadPool() 
{
    {
      std::unique_lock<std::mutex> lock(mutex);
      stop = true;
    }
 condition.notify_all();
 for (std::thread& worker : workers) 
    {
      worker.join();
    }
}

void ThreadPool::stopThreads()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }
    condition.notify_all();
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) 
            {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}


void searchFileInDirectory(ThreadPool& threadPool,const std::string& directory, const std::string& fileName)
{
    
    for (const auto& entry : std::filesystem::directory_iterator(directory)) 
    {
        if (entry.is_directory()) 
        {
            threadPool.enqueue([&, entry, fileName]() 
                {
                    searchFileInDirectory(threadPool,entry.path().string(), fileName);
                });
        }
        else if (entry.is_regular_file() && entry.path().filename() == fileName)
        {
            std::cout << "Found file: " << entry.path() << std::endl;
            threadPool.stopThreads();
        }
    }
}

