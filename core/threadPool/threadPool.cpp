#include "threadPool.hpp"
#include <iostream>
/// @brief ThreadPool
/// @details ThreadPool
constexpr size_t TASK_THRESHOLD = SIZE_MAX;
constexpr size_t THREAD_THRESHOLD = 100;
constexpr size_t IDLE_TIME = 60; // seconds
ThreadPool::ThreadPool() : mInitThreadSize(0), mThreadThreshold(THREAD_THRESHOLD), mTaskSize(0), mIdleThreadSize(0), mCurThreadSize(0), mQueThreshold(TASK_THRESHOLD), mMode(ThreadMode::FIXED), mIsRunning(false)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::start(size_t initSize)
{
    mIsRunning = true;
    mInitThreadSize = initSize;
    mCurThreadSize = initSize;
    for (size_t i = 0; i < mInitThreadSize; ++i)
    {
        // when create thread object, give thread function to thread object
        auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
        mThreads.emplace(ptr->getThreadID(), std::move(ptr));
    }

    for (auto &thread : mThreads)
    {
        thread.second->start();
        mIdleThreadSize++;
    }
}

void ThreadPool::setMode(ThreadMode mode)
{
    if (checkRunning())
        return;
    mMode = mode;
}

void ThreadPool::setTaskThreshold(size_t threshold)
{
    if (checkRunning())
        return;
    mQueThreshold = threshold;
}

void ThreadPool::setThreadThreshold(size_t threshold)
{
    if (checkRunning())
        return;
    if (mMode == ThreadMode::CACHE)
        mThreadThreshold = threshold;
}

Result ThreadPool::submitTask(std::shared_ptr<Task> task)
{
    // get lock
    std::unique_lock<std::mutex> tLock(mTaskQueMutex);

    // thread correspond, wait task queue not full
    // wait for 1s
    if (!mNotFull.wait_for(tLock, std::chrono::seconds(1), [&]()
                           { return mTaskQue.size() < mQueThreshold; }))
    {
        std::cerr << "Task queue is full, submit task failed!" << std::endl;
        return Result(task, false);
    }

    // if not full
    mTaskQue.emplace(task);
    mTaskSize++;
    // notify thread || because push task into queue, so notify thread to execute it
    mNotEmpty.notify_all();

    // cache | urgent, small, and fast task
    if (mMode == ThreadMode::CACHE && mTaskSize > mIdleThreadSize && mCurThreadSize < mThreadThreshold)
    {
        // if cache mode, create new thread to execute task
        auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
        mThreads.emplace(ptr->getThreadID(), std::move(ptr));
        mThreads[ptr->getThreadID()]->start();
        mCurThreadSize++;
        mIdleThreadSize++;
    }

    return Result(task);
}

void ThreadPool::threadFunc(size_t threadID)
{

    auto lastTime = std::chrono::high_resolution_clock().now();

    while (true)
    {
        std::shared_ptr<Task> task;
        {
            // get lock
            std::unique_lock<std::mutex> tLock(mTaskQueMutex);

            if (mMode == ThreadMode::CACHE)
            {
                // return per seconds
                while (mTaskQue.size() == 0)
                {
                    if (std::cv_status::timeout == mNotEmpty.wait_for(tLock, std::chrono::seconds(1)))
                    {
                        auto now = std::chrono::high_resolution_clock().now();
                        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);
                        if (duration.count() >= IDLE_TIME)
                        {
                            // callback thread
                            mThreads.erase(threadID);
                            mCurThreadSize--;
                            mIdleThreadSize--;
                            return;
                        }
                    }
                }
            }
            else
            {
                // wait task
                mNotEmpty.wait(tLock, [&]()
                               { return mTaskQue.size() > 0; });
            }
            mIdleThreadSize--;

            // get task
            task = mTaskQue.front();
            mTaskQue.pop();
            mTaskSize--;

            // correspond not empty,could pop task
            if (mTaskQue.size() > 0)
                mNotEmpty.notify_all();

            // correspond not full, could push task
            mNotFull.notify_all();
        } // release lock

        if (task != nullptr)
        {
            // execute task, use setValue give task return value to Result
            task->execute();
        }
        mIdleThreadSize++;
        lastTime = std::chrono::high_resolution_clock().now();
    }
}

bool ThreadPool::checkRunning() const
{
    return mIsRunning;
}

/// @brief Thread
/// @details Thread
size_t Thread::mGenerateID = 0;
Thread::Thread(ThreadFunc threadFunc) : mThreadFunc(threadFunc), mThreadID(mGenerateID++)
{
}
Thread::~Thread()
{
}
void Thread::start()
{
    // create thread
    std::thread worker(mThreadFunc, mThreadID); // worker use func
    worker.detach(); // when Thread::start() is over, worker thread will be detached, threadFunc can't be shut down.
}

size_t Thread::getThreadID() const
{
    return mThreadID;
}

/// @brief Result
/// @details Result
Result::Result(std::shared_ptr<Task> task, bool isValid) : mTask(task), mIsValid(isValid)
{
    mTask->setResult(this);
}

Any Result::get()
{
    if (!mIsValid)
    {
        throw "Result is invalid";
    }
    // wait task execute
    mSp.wait();
    // mAny is a lvalue, must use std::move to transfer to rvalue
    return std::move(mAny);
}

void Result::setValue(Any any)
{
    // store task result
    mAny = std::move(any);
    // have got task result, add semaphore
    mSp.post();
}

/// @brief Task
/// @details Task
Task::Task() : mResult(nullptr)
{
}

void Task::execute()
{
    if (mResult != nullptr)
    {
        mResult->setValue(run());
    }
}

void Task::setResult(Result *result)
{
    mResult = result;
}
