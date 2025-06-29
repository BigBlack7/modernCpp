#pragma once
#include <iostream>
#include <mutex>
#include <memory>

class Singleton
{
private:
    Singleton() {}
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

public:
    static const Singleton &getInstance()
    {
        static Singleton instance; // c++11 make sure local static var inits thread-safely
        return instance;
    }
    ~Singleton()
    {
        std::cout << "Singleton is destroyed" << std::endl;
    }
};
//---------------------------------------------
class Singleton2
{
private:
    Singleton2() = default;
    Singleton2(const Singleton2 &) = delete;
    Singleton2 &operator=(const Singleton2 &) = delete;

private:
    static std::shared_ptr<Singleton2> mPtr;

public:
    static std::shared_ptr<Singleton2> getInstance()
    {
        static std::once_flag flag;
        std::call_once(flag, []() { 
            mPtr = std::shared_ptr<Singleton2>(new Singleton2()); 
        });
        return mPtr;
    }
    ~Singleton2()
    {
        std::cout << "Singleton2 is destroyed" << std::endl;
    }
};
//---------------------------------------------
class Hungry
{
private:
    Hungry() {}
    Hungry(const Hungry &) = delete;
    Hungry &operator=(const Hungry &) = delete;

private:
    static Hungry *mPtr; // init in un-inline file

public:
    static Hungry *getInstance()
    {
        if (mPtr == nullptr)
            mPtr = new Hungry();
        return mPtr;
    }
    ~Hungry()
    {
        if (mPtr != nullptr)
            delete mPtr;
        std::cout << "Hungry is destroyed" << std::endl;
    }
};

//---------------------------------------------
class Lazy
{
private:
    Lazy() {}
    Lazy(const Lazy &) = delete;
    Lazy &operator=(const Lazy &) = delete;

private:
    static Lazy *mPtr; // init in un-inline file
    static std::mutex mLock;

public:
    static Lazy *getInstance()
    {
        std::lock_guard<std::mutex> lock(mLock);
        if (mPtr == nullptr)
            mPtr = new Lazy();
        return mPtr;
    }
    ~Lazy()
    {
        if (mPtr != nullptr)
            delete mPtr;
        std::cout << "Lazy is destroyed" << std::endl;
    }
};