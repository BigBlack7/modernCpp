#include "zMemoryPool.hpp"

ZMemoryPool::ZMemoryPool(size_t objSize, size_t poolSize)
    : mObjSize(objSize), mTotalSize(poolSize), mPool(new char[poolSize * objSize])
{
    if (mPool == nullptr)
        throw std::bad_alloc();

    for (size_t i = 0; i < poolSize; ++i)
    {
        mFreeList.push(mPool + i * objSize);
    }
}

ZMemoryPool::~ZMemoryPool()
{
    delete[] mPool;
}

void *ZMemoryPool::allocate()
{
    if (mFreeList.empty())
    {
        throw std::bad_alloc();
    }
    void *ptr = mFreeList.top();
    mFreeList.pop();
    return ptr;
}

void ZMemoryPool::deallocate(void *ptr)
{
    mFreeList.push((char *)ptr);
}
