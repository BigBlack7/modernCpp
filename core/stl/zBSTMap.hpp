#pragma once
#include <iostream>
#include <utility>
#include <exception>
#include <stack>

template <typename Key, typename Value>
struct ZBSTreeNode
{
    std::pair<Key, Value> __data__;
    ZBSTreeNode *__left__;
    ZBSTreeNode *__right__;
    ZBSTreeNode *__parent__;
    ZBSTreeNode(const Key &key, const Value &val, ZBSTreeNode *parent = nullptr)
        : __data__(std::make_pair(key, val)), __left__(nullptr), __right__(nullptr), __parent__(parent) {}
};

template <typename Key, typename Value>
class ZBSTMap
{
private:
    ZBSTreeNode<Key, Value> *mRoot;
    // delete all nodes
    void clear(ZBSTreeNode<Key, Value> *root)
    {
        if (root == nullptr)
            return;
        clear(root->__left__);
        clear(root->__right__);
        delete root;
    }
    // find min value of the node
    ZBSTreeNode<Key, Value> *minimum(ZBSTreeNode<Key, Value> *node) const
    {
        while (node->__left__ != nullptr)
        {
            node = node->__left__;
        }
        return node;
    }
    // find max value of the node
    ZBSTreeNode<Key, Value> *maximum(ZBSTreeNode<Key, Value> *node) const
    {
        while (node->__right__ != nullptr)
        {
            node = node->__right__;
        }
        return node;
    }
    // find successor of the node
    ZBSTreeNode<Key, Value> *successor(ZBSTreeNode<Key, Value> *node) const
    {
        if (node->__right__ != nullptr)
        {
            return minimum(node->__right__);
        }

        ZBSTreeNode<Key, Value> *parent = node->__parent__;
        while (parent != nullptr && node == parent->__right__)
        {
            node = parent;
            parent = parent->__parent__;
        }
        return parent;
    }

public:
    ZBSTMap() : mRoot(nullptr) {}
    ~ZBSTMap()
    {
        clear();
    }
    void clear()
    {
        clear(mRoot);
        mRoot = nullptr;
    }
    ZBSTMap(const ZBSTMap &) = delete;
    ZBSTMap &operator=(const ZBSTMap &) = delete;
    Value &operator[](const Key &key)
    {
        ZBSTreeNode<Key, Value> *node = find(key);
        if (node)
        {
            return node->__data__.second;
        }
        else
        {
            insert(key, Value());
            return find(key)->__data__.second;
        }
    }

    // Accroding to key to insert value
    void insert(const Key &key, const Value &val)
    {
        if (mRoot == nullptr)
        {
            mRoot = new ZBSTreeNode<Key, Value>(key, val);
            return;
        }
        ZBSTreeNode<Key, Value> *current = mRoot;
        ZBSTreeNode<Key, Value> *parent = nullptr;
        while (current != nullptr)
        {
            parent = current;
            if (key < current->__data__.first)
            {
                current = current->__left__;
            }
            else if (key > current->__data__.first)
            {
                current = current->__right__;
            }
            else
            {
                current->__data__.second = val;
                return;
            }
        }
        if (key < parent->__data__.first)
        {
            parent->__left__ = new ZBSTreeNode<Key, Value>(key, val, parent);
        }
        else
        {
            parent->__right__ = new ZBSTreeNode<Key, Value>(key, val, parent);
        }
    }

    // According to key to find value
    ZBSTreeNode<Key, Value> *find(const Key &key) const
    {
        ZBSTreeNode<Key, Value> *current = mRoot;
        while (current != nullptr)
        {
            if (key < current->__data__.first)
            {
                current = current->__left__;
            }
            else if (key > current->__data__.first)
            {
                current = current->__right__;
            }
            else
            {
                return current;
            }
        }
        return nullptr;
    }

    void erase(const Key &key)
    {
        ZBSTreeNode<Key, Value> *node = find(key);
        if (node == nullptr)
            return;
        // left and right all exist
        if (node->__left__ != nullptr && node->__right__ != nullptr)
        {
            ZBSTreeNode<Key, Value> *successor = minimum(node->__right__);
            node->__data__ = successor->__data__;
            node = successor;
        }

        // refresh child's parent
        ZBSTreeNode<Key, Value> *child = (node->__left__) ? node->__left__ : node->__right__;
        if (child != nullptr)
        {
            child->__parent__ = node->__parent__;
        }

        if (node->__parent__ == nullptr) // if delete root
        {
            mRoot = child;
        }
        // Accroding to the relationship of node and parent to get node's child new place
        else if (node == node->__parent__->__left__)
        {
            node->__parent__->__left__ = child;
        }
        else
        {
            node->__parent__->__right__ = child;
        }

        delete node;
    }

    class ZBSTMapIterator
    {
    private:
        ZBSTreeNode<Key, Value> *mCurrent;
        // find min value of the node
        ZBSTreeNode<Key, Value> *minimum(ZBSTreeNode<Key, Value> *node) const
        {
            while (node->__left__ != nullptr)
            {
                node = node->__left__;
            }
            return node;
        }
        // find max value of the node
        ZBSTreeNode<Key, Value> *maximum(ZBSTreeNode<Key, Value> *node) const
        {
            while (node->__right__ != nullptr)
            {
                node = node->__right__;
            }
            return node;
        }
        // find successor of the node
        ZBSTreeNode<Key, Value> *successor(ZBSTreeNode<Key, Value> *node) const
        {
            if (node->__right__ != nullptr)
            {
                return minimum(node->__right__);
            }

            ZBSTreeNode<Key, Value> *parent = node->__parent__;
            while (parent != nullptr && node == parent->__right__)
            {
                node = parent;
                parent = parent->__parent__;
            }
            return parent;
        }

    public:
        ZBSTMapIterator(ZBSTreeNode<Key, Value> *node) : mCurrent(node) {}
        std::pair<Key, Value> &operator*() const { return mCurrent->__data__; }
        std::pair<Key, Value> *operator->() const { return &(mCurrent->__data__); }
        bool operator==(const ZBSTMapIterator &other) const { return mCurrent == other.mCurrent; }
        bool operator!=(const ZBSTMapIterator &other) const { return !(*this == other); }
        ZBSTMapIterator &operator++()
        {
            mCurrent = successor(mCurrent);
            return *this;
        }
        ZBSTMapIterator operator++(int)
        {
            ZBSTMapIterator temp = *this;
            ++(*this);
            return temp;
        }
        ZBSTMapIterator &operator--()
        {
            mCurrent = predecessor(mCurrent);
            return *this;
        }
        ZBSTMapIterator operator--(int)
        {
            ZBSTMapIterator temp = *this;
            --(*this);
            return temp;
        }
    };

    ZBSTMapIterator begin() const
    {
        return ZBSTMapIterator(minimum(mRoot));
    }

    ZBSTMapIterator end() const
    {
        return ZBSTMapIterator(nullptr);
    }

    void print() const
    {
        for (auto it = begin(); it != end(); ++it)
        {
            std::cout << it->first << "=>" << it->second << std::endl;
        }
    }
};
