#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <chrono>

class Any // type trap, used to store any type data | c++17
{
public:
    Any() = default;
    ~Any() = default;
    Any(const Any &) = delete;
    Any &operator=(const Any &) = delete;
    Any(Any &&) = default;
    Any &operator=(Any &&) = default;

    template <typename T>
    Any(T data) : mBase(std::make_unique<Derive<T>>(data)) {}

    template <typename T>
    T cast()
    {
        Derive<T> *derive = dynamic_cast<Derive<T> *>(mBase.get());
        if (derive == nullptr)
        {
            throw "type not match";
        }
        return derive->mData;
    }

private:
    class Base
    {
    public:
        virtual ~Base() = default;
    };
    template <typename T>
    class Derive : public Base
    {
    public:
        Derive(T data) : mData(data) {}
        T mData;
    };

private:
    std::unique_ptr<Base> mBase;
};

class Semaphore // c++20
{
public:
    Semaphore(int limit = 0) : mResLimit(limit) {}
    ~Semaphore() = default;

    // get semaphore
    void wait()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCond.wait(lock, [&]() -> bool
                   { return mResLimit > 0; });
        mResLimit--;
    }

    // add semaphore
    void post()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mResLimit++;
        mCond.notify_all();
    }

private:
    int mResLimit;
    std::mutex mMutex;
    std::condition_variable mCond;
};

class Result;
class Task
{
public:
    Task();
    ~Task() = default;
    void execute();
    virtual Any run() = 0;
    void setResult(Result *result);

private:
    Result *mResult;
};

class Result
{
public:
    Result(std::shared_ptr<Task> task, bool isValid = true);
    ~Result() = default;
    Any get();
    void setValue(Any any);

private:
    Any mAny;
    Semaphore mSp;
    std::shared_ptr<Task> mTask;
    std::atomic_bool mIsValid;
};

enum class ThreadMode
{
    FIXED, // thread nums fixed
    CACHE  // thread nums vary
};

class Thread
{
public:
    using ThreadFunc = std::function<void(size_t)>;
    Thread(ThreadFunc threadFunc);
    ~Thread();
    void start();
    size_t getThreadID() const;

private:
    ThreadFunc mThreadFunc;
    static size_t mGenerateID;
    size_t mThreadID;
};

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    void start(size_t initSize = 4);
    void setMode(ThreadMode mode);
    void setTaskThreshold(size_t threshold);
    void setThreadThreshold(size_t threshold); // cache mode
    Result submitTask(std::shared_ptr<Task> task);

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

private:
    void threadFunc(size_t threadID); // define thread function
    bool checkRunning() const;

private:
    // std::vector<std::unique_ptr<Thread>> mThreads;
    std::unordered_map<size_t, std::unique_ptr<Thread>> mThreads; // thread list
    size_t mInitThreadSize;                                      // init thread nums
    size_t mThreadThreshold;                                     // max thread nums
    std::atomic_size_t mIdleThreadSize;
    std::atomic_size_t mCurThreadSize;

    std::queue<std::shared_ptr<Task>> mTaskQue;
    std::atomic_uint mTaskSize; // make sure task num operations thread safe
    size_t mQueThreshold;       // max task

    std::mutex mTaskQueMutex; // protect task queue thread safe
    std::condition_variable mNotFull;
    std::condition_variable mNotEmpty;
    ThreadMode mMode;
    std::atomic_bool mIsRunning;
};