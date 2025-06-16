#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <functional>

template <typename Key, typename Value>
struct ZAVLNode
{
    Key __key__;
    Value __value__;
    int __height__;
    ZAVLNode *__left__;
    ZAVLNode *__right__;
    ZAVLNode(const Key &key, const Value &value)
        : __key__(key), __value__(value), __height__(1), __left__(nullptr), __right__(nullptr) {}
};

template <typename Key, typename Value>
class ZAVLMap
{
private:
    ZAVLNode<Key, Value> *mRoot;

private:
    int getHeight(ZAVLNode<Key, Value> *node)
    {
        if (node == nullptr)
            return 0;
        return node->__height__;
    }

    int getBalanceFactor(ZAVLNode<Key, Value> *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->__left__) - getHeight(node->__right__);
    }

    ZAVLNode<Key, Value> *rightRotate(ZAVLNode<Key, Value> *node)
    {
        ZAVLNode<Key, Value> *left = node->__left__;
        node->__left__ = left->__right__;
        left->__right__ = node;
        node->__height__ = std::max(getHeight(node->__left__), getHeight(node->__right__)) + 1;
        left->__height__ = std::max(getHeight(left->__left__), getHeight(left->__right__)) + 1;
        return left;
    }
    ZAVLNode<Key, Value> *leftRotate(ZAVLNode<Key, Value> *node)
    {
        ZAVLNode<Key, Value> *right = node->__right__;
        node->__right__ = right->__left__;
        right->__left__ = node;
        node->__height__ = std::max(getHeight(node->__left__), getHeight(node->__right__)) + 1;
        right->__height__ = std::max(getHeight(right->__left__), getHeight(right->__right__)) + 1;
        return right;
    }

    ZAVLNode<Key, Value> *minValueNode(ZAVLNode<Key, Value> *node)
    {
        ZAVLNode<Key, Value> *current = node;
        while (current->__left__ != nullptr)
            current = current->__left__;
        return current;
    }

    ZAVLNode<Key, Value> *insert(ZAVLNode<Key, Value> *node, const Key &key, const Value &value)
    {
        if (node == nullptr)
            return new ZAVLNode<Key, Value>(key, value);
        if (key < node->__key__)
            node->__left__ = insert(node->__left__, key, value);
        else if (key > node->__key__)
            node->__right__ = insert(node->__right__, key, value);
        else
            node->__value__ = value;

        node->__height__ = std::max(getHeight(node->__left__), getHeight(node->__right__)) + 1;
        int bias = getBalanceFactor(node);
        // LL
        if (bias > 1 && key < node->__left__->__key__)
            return rightRotate(node);
        // RR
        if (bias < -1 && key > node->__right__->__key__)
            return leftRotate(node);
        // RL
        if (bias < -1 && key < node->__right__->__key__)
        {
            node->__right__ = rightRotate(node->__right__);
            return leftRotate(node);
        }
        // LR
        if (bias > 1 && key > node->__left__->__key__)
        {
            node->__left__ = leftRotate(node->__left__);
            return rightRotate(node);
        }
        return node;
    }

    ZAVLNode<Key, Value> *remove(ZAVLNode<Key, Value> *node, const Key &key)
    {
        if (node == nullptr)
            return node;
        if (key < node->__key__)
            node->__left__ = remove(node->__left__, key);
        else if (key > node->__key__)
            node->__right__ = remove(node->__right__, key);
        else
        {
            if (node->__left__ == nullptr || node->__right__ == nullptr)
            {
                ZAVLNode<Key, Value> *temp = node->__left__ ? node->__left__ : node->__right__;
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                delete temp;
            }
            else
            {
                ZAVLNode<Key, Value> *temp = minValueNode(node->__right__);
                node->__key__ = temp->__key__;
                node->__value__ = temp->__value__;
                node->__right__ = remove(node->__right__, temp->__key__);
            }
        }

        if (node == nullptr)
            return node;
        node->__height__ = std::max(getHeight(node->__left__), getHeight(node->__right__)) + 1;
        int bias = getBalanceFactor(node);
        // LL
        if (bias > 1 && getBalanceFactor(node->__left__) >= 0)
            return rightRotate(node);
        // RR
        if (bias < -1 && getBalanceFactor(node->__right__) <= 0)
            return leftRotate(node);
        // RL
        if (bias < -1 && getBalanceFactor(node->__right__) > 0)
        {
            node->__right__ = leftRotate(node->__right__);
            return rightRotate(node);
        }
        // LR
        if (bias > 1 && getBalanceFactor(node->__left__) < 0)
        {
            node->__left__ = rightRotate(node->__left__);
            return leftRotate(node);
        }
        return node;
    }

    Value *find(ZAVLNode<Key, Value> *node, const Key &key)
    {
        if (node == nullptr)
            return nullptr;
        if (key < node->__key__)
            return find(node->__left__, key);
        else if (key > node->__key__)
            return find(node->__right__, key);
        else
            return &node->__value__;
    }

    void inorder(ZAVLNode<Key, Value> *node, std::vector<std::pair<Key, Value>> &result)
    {
        if (node == nullptr)
            return;
        inorder(node->__left__, result);
        result.push_back(std::make_pair(node->__key__, node->__value__));
        inorder(node->__right__, result);
    }

public:
    ZAVLMap() : mRoot(nullptr) {}

    ~ZAVLMap()
    {
        std::function<void(ZAVLNode<Key, Value> *)> destroy = [&](ZAVLNode<Key, Value> *node)
        {
            if (node == nullptr)
                return;
            destroy(node->__left__);
            destroy(node->__right__);
            delete node;
        };
        destroy(mRoot);
    }

    void put(const Key &key, const Value &value)
    {
        mRoot = insert(mRoot, key, value);
    }

    Value *get(const Key &key)
    {
        return find(mRoot, key);
    }

    void erase(const Key &key)
    {
        mRoot = remove(mRoot, key);
    }

    std::vector<std::pair<Key, Value>> inorderTraversal()
    {
        std::vector<std::pair<Key, Value>> result;
        inorder(mRoot, result);
        return result;
    }
};