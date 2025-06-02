#pragma once
#include <mutex>
#include <atomic>
// The declaration and implementation of a template class cannot be separated, otherwise a linking error will occur
struct ControlBlock
{
    std::atomic<int> __refCount__; // Reference count
    ControlBlock() : __refCount__(1) {}
    // in fact,ptr should be here -> T *mPtr;
};

template <typename T>
class ZSharedPtr
{
private:
    T *mPtr; // Resource management pointer
    ControlBlock *mControlBlock;
    void release();

public:
    ZSharedPtr();
    explicit ZSharedPtr(T *ptr);
    ~ZSharedPtr();

    ZSharedPtr(const ZSharedPtr<T> &other);               // Copy constructor
    ZSharedPtr<T> &operator=(const ZSharedPtr<T> &other); // Copy assignment operator overload

    ZSharedPtr(ZSharedPtr<T> &&other) noexcept;               // Move constructor
    ZSharedPtr<T> &operator=(ZSharedPtr<T> &&other) noexcept; // Move assignment operator overload

    T *operator->() const; // Overload the arrow operator, return the resource management pointer
    T &operator*() const;  // Overload the dereference operator, return the object pointed to by the resource management pointer

    int use_count() const;        // Return the reference count
    T *get() const;               // Return the resource management pointer
    void reset(T *ptr = nullptr); // Reset the resource management pointer
};

template <typename T>
void ZSharedPtr<T>::release()
{
    if (mControlBlock)
    {
        --mControlBlock->__refCount__; // Decrement the reference count
        if (mControlBlock->__refCount__ == 0)
        {
            // If the reference count is 0, release the resource
            delete mPtr;
            mPtr = nullptr;
            delete mControlBlock;
            mControlBlock = nullptr;
        }
    }
}

template <typename T>
ZSharedPtr<T>::ZSharedPtr() : mPtr(nullptr), mControlBlock(nullptr) {}

template <typename T>
ZSharedPtr<T>::ZSharedPtr(T *ptr) : mPtr(ptr)
{
    if (ptr)
        mControlBlock = new ControlBlock();
    else
        mControlBlock = nullptr;
}

template <typename T>
ZSharedPtr<T>::~ZSharedPtr()
{
    if (mPtr)
        release();
}

template <typename T>
ZSharedPtr<T>::ZSharedPtr(const ZSharedPtr<T> &other) : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
{
    if (mControlBlock)
        mControlBlock->__refCount__++;
}

template <typename T>
ZSharedPtr<T> &ZSharedPtr<T>::operator=(const ZSharedPtr<T> &other)
{
    if (this != &other)
    {
        release();
        mPtr = other.mPtr;
        mControlBlock = other.mControlBlock;
        if (mControlBlock)
            mControlBlock->__refCount__++;
    }
    return *this;
}

template <typename T>
ZSharedPtr<T>::ZSharedPtr(ZSharedPtr<T> &&other) noexcept : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
{
    other.mPtr = nullptr;
    other.mControlBlock = nullptr;
}

template <typename T>
ZSharedPtr<T> &ZSharedPtr<T>::operator=(ZSharedPtr<T> &&other) noexcept
{
    if (this != &other)
    {
        release();
        mPtr = other.mPtr;
        mControlBlock = other.mControlBlock;
        other.mPtr = nullptr;
        other.mControlBlock = nullptr;
    }
    return *this;
}

template <typename T>
T *ZSharedPtr<T>::operator->() const
{
    return mPtr;
}

template <typename T>
T &ZSharedPtr<T>::operator*() const
{
    return *mPtr;
}

template <typename T>
int ZSharedPtr<T>::use_count() const
{
    return mControlBlock ? mControlBlock->__refCount__.load() : 0;
}

template <typename T>
T *ZSharedPtr<T>::get() const
{
    return mPtr;
}

template <typename T>
void ZSharedPtr<T>::reset(T *ptr)
{
    release();
    mPtr = ptr;
    if (ptr)
        mControlBlock = new ControlBlock();
    else
        mControlBlock = nullptr;
}
