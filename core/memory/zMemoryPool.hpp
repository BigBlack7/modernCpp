#pragma once
#include <iostream>
#include <stack>

class ZMemoryPool
{
private:
    size_t mObjSize;
    size_t mTotalSize;
    char *mPool;
    std::stack<void *> mFreeList;

public:
    ZMemoryPool(size_t objSize, size_t poolSize);
    ~ZMemoryPool();

    void *allocate();
    void deallocate(void *ptr);
};