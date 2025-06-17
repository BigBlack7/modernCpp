#pragma once
#include <iostream>
#include <utility>
#include <functional>
#include <list>
#include <vector>
#include <iterator>
#include <stdexcept>

template <typename Key, typename Value>
struct ZHashNode
{
    std::pair<Key, Value> __data__;
    ZHashNode *__next__;
    ZHashNode(const Key &key, const Value &value) : __data__(std::make_pair(key, value)), __next__(nullptr) {}
};

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class ZHashMap
{
public:
    class Iterator;
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const Key, Value>;

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<Key, Value>;
        using pointer = value_type *;
        using reference = value_type &;

    public:
        Iterator(ZHashMap *map, size_t bucketIndex, ZHashNode<Key, Value> *node)
            : mMap(map), mBucketIndex(bucketIndex), mCurrentNode(node) {}

        reference operator*() const
        {
            if (mCurrentNode == nullptr)
            {
                throw std::out_of_range("Iterator dereference out of range");
            }
            return mCurrentNode->__data__;
        }

        pointer operator->() const
        {
            if (mCurrentNode == nullptr)
            {
                throw std::out_of_range("Iterator dereference out of range");
            }
            return &mCurrentNode->__data__;
        }

        Iterator &operator++()
        {
            if (mCurrentNode == nullptr)
            {
                throw std::out_of_range("Iterator increment out of range");
            }
            mCurrentNode = mCurrentNode->__next__;
            if (mCurrentNode == nullptr)
            {
                advance();
            }
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator &other) const
        {
            return mMap == other.mMap && mBucketIndex == other.mBucketIndex && mCurrentNode == other.mCurrentNode;
        }

        bool operator!=(const Iterator &other) const
        {
            return !(*this == other);
        }

    private:
        ZHashMap *mMap;
        size_t mBucketIndex;
        ZHashNode<Key, Value> *mCurrentNode;

    private:
        void advance()
        {
            if (mCurrentNode != nullptr)
            {
                mCurrentNode = mCurrentNode->__next__;
            }
            while (mCurrentNode == nullptr)
            {
                if (mBucketIndex + 1 < mMap->mBucketCount)
                {
                    mBucketIndex++;
                    mCurrentNode = mMap->mBuckets[mBucketIndex];
                }
                else if (mBucketIndex + 1 == mMap->mBucketCount)
                {
                    ++mBucketIndex;
                    break;
                }
            }
        }
    };

public:
    ZHashMap(size_t bucketCount = 16, double loadFactor = 0.75)
        : mBucketCount(bucketCount), mElementCount(0), mLoadFactor(loadFactor), mHashFunction(Hash())
    {
        mBuckets.resize(bucketCount, nullptr);
    }

    ~ZHashMap()
    {
        clear();
    }

    ZHashMap(const ZHashMap &other) = delete;
    ZHashMap &operator=(const ZHashMap &other) = delete;

    void insert(const Key &key, const Value &value)
    {
        size_t hashValue = mHashFunction(key);
        size_t index = hashValue % mBucketCount;

        ZHashNode<Key, Value> *node = mBuckets[index];
        while (node != nullptr)
        {
            if (node->__data__.first == key)
            {
                node->__data__.second = value;
                return;
            }
            node = node->__next__;
        }

        ZHashNode<Key, Value> *newNode = new ZHashNode<Key, Value>(key, value);
        newNode->__next__ = mBuckets[index];
        mBuckets[index] = newNode;
        mElementCount++;
        double loadFactor = static_cast<double>(mElementCount) / mBucketCount;
        if (loadFactor > mLoadFactor)
        {
            refresh();
        }
    }

    Value *find(const Key &key)
    {
        size_t hashValue = mHashFunction(key);
        size_t index = hashValue % mBucketCount;

        ZHashNode<Key, Value> *node = mBuckets[index];
        while (node != nullptr)
        {
            if (node->__data__.first == key)
            {
                return &node->__data__.second;
            }
            node = node->__next__;
        }
        return nullptr;
    }

    bool erase(const Key &key)
    {
        size_t hashValue = mHashFunction(key);
        size_t index = hashValue % mBucketCount;

        ZHashNode<Key, Value> *node = mBuckets[index];
        ZHashNode<Key, Value> *prev = nullptr;
        while (node != nullptr)
        {
            if (node->__data__.first == key)
            {
                if (prev == nullptr)
                {
                    mBuckets[index] = node->__next__;
                }
                else
                {
                    prev->__next__ = node->__next__;
                }
                delete node;
                mElementCount--;
                return true;
            }
            prev = node;
            node = node->__next__;
        }
        return false;
    }

    size_t size() const
    {
        return mElementCount;
    }
    bool empty() const
    {
        return mElementCount == 0;
    }

    void clear()
    {
        for (auto &bucket : mBuckets)
        {
            ZHashNode<Key, Value> *current = bucket;
            while (current != nullptr)
            {
                ZHashNode<Key, Value> *temp = current;
                current = current->__next__;
                delete temp;
            }
            bucket = nullptr;
        }
        mElementCount = 0;
    }

    Iterator begin()
    {
        size_t bucketIndex = 0;
        while (bucketIndex < mBucketCount && mBuckets[bucketIndex] == nullptr)
        {
            bucketIndex++;
        }
        return Iterator(this, bucketIndex, mBuckets[bucketIndex]);
    }
    Iterator end()
    {
        return Iterator(this, mBucketCount, nullptr);
    }

private:
    std::vector<ZHashNode<Key, Value> *> mBuckets;
    size_t mBucketCount;
    size_t mElementCount;
    double mLoadFactor;
    Hash mHashFunction;

private:
    void refresh()
    {
        auto newBucketCount = mBucketCount * 2;
        std::vector<ZHashNode<Key, Value> *> newBuckets(newBucketCount, nullptr);

        for (auto &bucket : mBuckets)
        {
            ZHashNode<Key, Value> *current = bucket;
            while (current != nullptr)
            {
                size_t hashValue = mHashFunction(current->__data__.first);
                size_t index = hashValue % newBucketCount;

                ZHashNode<Key, Value> *nextNode = current->__next__;
                current->__next__ = newBuckets[index];
                newBuckets[index] = current;
                current = nextNode;
            }
        }

        mBuckets = std::move(newBuckets);
        mBucketCount = newBucketCount;
    }
};