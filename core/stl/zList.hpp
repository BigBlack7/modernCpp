#pragma once
#include <iostream>

template <typename T>
struct ZNode
{
    T __data__;
    ZNode *__prev__;
    ZNode *__next__;

    ZNode(const T &data = T()) : __data__(data), __prev__(nullptr), __next__(nullptr) {}
};

template <typename T>
class ZList;

template <typename T>
class ZIterator
{
private:
    ZNode<T> *mNodePtr;
    friend class ZList<T>;

public:
    using self_type = ZIterator<T>;
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;

    ZIterator(ZNode<T> *ptr = nullptr) : mNodePtr(ptr) {}

    // Dereference operator overload
    reference operator*() const { return mNodePtr->__data__; }

    // Arrow operator overload
    pointer operator->() const { return &(mNodePtr->__data__); }

    // Pre-increment operator overload
    self_type &operator++()
    {
        if (mNodePtr)
            mNodePtr = mNodePtr->__next__;
        return *this;
    }

    // Post-increment operator overload
    self_type operator++(int)
    {
        self_type temp = *this;
        ++(*this);
        return temp;
    }

    // Pre-decrement operator overload
    self_type &operator--()
    {
        if (mNodePtr)
            mNodePtr = mNodePtr->__prev__;
        return *this;
    }

    // Post-decrement operator overload
    self_type operator--(int)
    {
        self_type temp = *this;
        --(*this);
        return temp;
    }

    // Equality operator overload
    bool operator==(const self_type &other) const { return mNodePtr == other.mNodePtr; }
    // Inequality operator overload
    bool operator!=(const self_type &other) const { return mNodePtr != other.mNodePtr; }
};

template <typename T>
class ZList
{
private:
    ZNode<T> *mHead;
    ZNode<T> *mTail;

public:
    using iterator = ZIterator<T>;
    using const_iterator = ZIterator<const T>;

    // Default constructor
    ZList()
    {
        mHead = new ZNode<T>();
        mTail = new ZNode<T>();
        mHead->__next__ = mTail;
        mTail->__prev__ = mHead;
    }

    // Destructor
    ~ZList()
    {
        clear();
        delete mHead;
        delete mTail;
    }

    // delete all elements in the list
    void clear()
    {
        ZNode<T> *current = mHead->__next__;
        while (current != mTail)
        {
            ZNode<T> *temp = current;
            current = current->__next__;
            delete temp;
        }
        mHead->__next__ = mTail;
        mTail->__prev__ = mHead;
    }

    // forbid copy
    ZList(const ZList &) = delete;
    ZList &operator=(const ZList &) = delete;

    // head and tail iterator
    iterator begin() { return iterator(mHead->__next__); }
    iterator end() { return iterator(mTail); }

    // head and tail element
    T &front() { return mHead->__next__->__data__; }
    T &back() { return mTail->__prev__->__data__; }

    // element size or empty
    bool empty() const { return mHead->__next__ == mTail; }
    size_t size() const
    {
        size_t count = 0;
        for (auto it = begin(); it != end(); ++it)
            ++count;
        return count;
    }

    // insert element
    iterator insert(iterator pos, const T &value)
    {
        ZNode<T> *current = pos.mNodePtr;
        ZNode<T> *newNode = new ZNode<T>(value);

        ZNode<T> *prevNode = current->__prev__;
        newNode->__next__ = current;
        newNode->__prev__ = prevNode;

        prevNode->__next__ = newNode;
        current->__prev__ = newNode;
        return iterator(newNode);
    }

    // erase element
    iterator erase(iterator pos)
    {
        ZNode<T> *current = pos.mNodePtr;
        if (current == mHead || current == mTail)
        {
            return pos; // cannot erase head or tail
        }
        ZNode<T> *prevNode = current->__prev__;
        ZNode<T> *nextNode = current->__next__;
        prevNode->__next__ = nextNode;
        nextNode->__prev__ = prevNode;
        delete current;
        return iterator(nextNode);
    }

    // remove element with value
    void remove(const T &value)
    {
        for (auto it = begin(); it != end();)
        {
            if (*it == value)
                it = erase(it);
            else
                ++it;
        }
    }

    // insert or delete element at the front of the list
    void push_front(const T &value) { insert(begin(), value); }
    void pop_front()
    {
        if (!empty())
            erase(begin());
    }

    // insert or delete element at the back of the list
    void push_back(const T &value) { insert(end(), value); }
    void pop_back()
    {
        if (!empty())
        {
            iterator temp = end();
            --temp;
            erase(temp);
        }
    }

    // print all elements in the list
    void print() const
    {
        ZNode<T> *current = mHead->__next__;
        while (current != mTail)
        {
            std::cout << current->__data__ << " ";
            current = current->__next__;
        }
        std::cout << std::endl;
    }
};