#pragma once
#include <iostream>

template <typename T>
class ZDeque
{
private:
    T *mBuffer;
    std::size_t mCapacity;
    std::size_t mCount;
    std::size_t mFrontIdx;
    std::size_t mBackIdx;

public:
    ZDeque(size_t initial_capacity = 10) : mCapacity(initial_capacity), mCount(0), mFrontIdx(0), mBackIdx(0)
    {
        mBuffer = new T[mCapacity]();
    }

    ~ZDeque()
    {
        delete[] mBuffer;
    }

    bool empty() const
    {
        return mCount == 0;
    }

    size_t size() const
    {
        return mCount;
    }

    // reAlloc buffer to new capacity
    void resize(size_t new_capacity)
    {
        if (new_capacity <= mCapacity)
            return;
        T *new_buffer = new T[new_capacity]();
        for (size_t i = 0; i < mCount; ++i)
            new_buffer[i] = mBuffer[(mFrontIdx + i) % mCapacity];
        delete[] mBuffer;
        mBuffer = new_buffer;
        mCapacity = new_capacity;
        mFrontIdx = 0;
        mBackIdx = mCount;
    }

    // front insert element
    void push_front(const T &value)
    {
        if (mCount == mCapacity)
            resize(mCapacity * 2);
        (mFrontIdx == 0) ? (mFrontIdx = mCapacity - 1) : (--mFrontIdx);
        mBuffer[mFrontIdx] = value;
        ++mCount;
    }
    // back insert element
    void push_back(const T &value)
    {
        if (mCount == mCapacity)
            resize(mCapacity * 2);
        mBuffer[mBackIdx] = value;
        mBackIdx = (mBackIdx + 1) % mCapacity;
        ++mCount;
    }

    // front pop element
    void pop_front()
    {
        if (empty())
            throw std::out_of_range("Deque is empty!");
        mFrontIdx = (mFrontIdx + 1) % mCapacity;
        --mCount;
    }
    // back pop element
    void pop_back()
    {
        if (empty())
            throw std::out_of_range("Deque is empty!");
        (mBackIdx == 0) ? (mBackIdx = mCapacity - 1) : (--mBackIdx);
        --mCount;
    }

    // front element and back element
    T &front()
    {
        if (empty())
            throw std::out_of_range("Deque is empty!");
        return mBuffer[mFrontIdx];
    }
    T &back()
    {
        if (empty())
            throw std::out_of_range("Deque is empty!");
        return mBuffer[(mBackIdx == 0) ? (mCapacity - 1) : (mBackIdx - 1)];
    }
    const T &front() const
    {
        if (empty())
            throw std::out_of_range("Deque is empty!");
        return mBuffer[mFrontIdx];
    }
    const T &back() const
    {
        if (empty())
            throw std::out_of_range("Deque is empty!");
        return mBuffer[(mBackIdx == 0) ? (mCapacity - 1) : (mBackIdx - 1)];
    }

    // print all elements in the deque
    void print() const
    {
        for (size_t i = 0; i < mCount; ++i)
            std::cout << mBuffer[(mFrontIdx + i) % mCapacity] << " ";
        std::cout << std::endl;
    }

    // iterator
    class ZDequeIterator
    {
    private:
        ZDeque<T> *mDequePtr;
        std::size_t mPos;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        ZDequeIterator(ZDeque<T> *dequePtr, std::size_t pos) : mDequePtr(dequePtr), mPos(pos) {}

        reference operator*() const { return mDequePtr->mBuffer[(mDequePtr->mFrontIdx + mPos) % mDequePtr->mCapacity]; }
        pointer operator->() const { return &(mDequePtr->mBuffer[(mDequePtr->mFrontIdx + mPos) % mDequePtr->mCapacity]); }

        // pre-increment operator overload
        ZDequeIterator &operator++()
        {
            ++mPos;
            return *this;
        }
        // post-increment operator overload
        ZDequeIterator operator++(int)
        {
            ZDequeIterator temp = *this;
            ++mPos;
            return temp;
        }

        // pre-decrement operator overload
        ZDequeIterator &operator--()
        {
            --mPos;
            return *this;
        }
        // post-decrement operator overload
        ZDequeIterator operator--(int)
        {
            ZDequeIterator temp = *this;
            --mPos;
            return temp;
        }
        // equal operator overload
        bool operator==(const ZDequeIterator &other) const { return mDequePtr == other.mDequePtr && mPos == other.mPos; }
        // not equal operator overload
        bool operator!=(const ZDequeIterator &other) const { return !(*this == other); }
    };

    ZDequeIterator begin() { return ZDequeIterator(this, 0); }
    ZDequeIterator end() { return ZDequeIterator(this, mCount); }
};