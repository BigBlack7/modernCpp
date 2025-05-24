#include "zSharedPtr.hpp"

// template <typename T>
// void ZSharedPtr<T>::release()
// {
//     if (mControlBlock)
//     {
//         --mControlBlock->__refCount__; // 引用计数减一
//         if (mControlBlock->__refCount__ == 0)
//         {
//             // 如果引用计数为0，释放资源
//             delete mPtr;
//             mPtr = nullptr;
//             delete mControlBlock;
//             mControlBlock = nullptr;
//         }
//     }
// }

// template <typename T>
// ZSharedPtr<T>::ZSharedPtr() : mPtr(nullptr), mControlBlock(nullptr) {}

// template <typename T>
// ZSharedPtr<T>::ZSharedPtr(T *ptr) : mPtr(ptr)
// {
//     if (ptr)
//         mControlBlock = new ControlBlock();
//     else
//         mControlBlock = nullptr;
// }

// template <typename T>
// ZSharedPtr<T>::~ZSharedPtr()
// {
//     if (mPtr)
//         release();
// }

// template <typename T>
// ZSharedPtr<T>::ZSharedPtr(const ZSharedPtr<T> &other) : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
// {
//     if (mControlBlock)
//         mControlBlock->__refCount__++;
// }

// template <typename T>
// ZSharedPtr<T> &ZSharedPtr<T>::operator=(const ZSharedPtr<T> &other)
// {
//     if (this != &other)
//     {
//         release();
//         mPtr = other.mPtr;
//         mControlBlock = other.mControlBlock;
//         if (mControlBlock)
//             mControlBlock->__refCount__++;
//     }
//     return *this;
// }

// template <typename T>
// ZSharedPtr<T>::ZSharedPtr(ZSharedPtr<T> &&other) noexcept : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
// {
//     other.mPtr = nullptr;
//     other.mControlBlock = nullptr;
// }

// template <typename T>
// ZSharedPtr<T> &ZSharedPtr<T>::operator=(ZSharedPtr<T> &&other) noexcept
// {
//     if (this != &other)
//     {
//         release();
//         mPtr = other.mPtr;
//         mControlBlock = other.mControlBlock;
//         other.mPtr = nullptr;
//         other.mControlBlock = nullptr;
//     }
//     return *this;
// }

// template <typename T>
// T *ZSharedPtr<T>::operator->() const
// {
//     return mPtr;
// }

// template <typename T>
// T &ZSharedPtr<T>::operator*() const
// {
//     return *mPtr;
// }

// template <typename T>
// int ZSharedPtr<T>::use_count() const
// {
//     return mControlBlock ? mControlBlock->__refCount__ : 0;
// }

// template <typename T>
// T *ZSharedPtr<T>::get() const
// {
//     return mPtr;
// }

// template <typename T>
// void ZSharedPtr<T>::reset(T *ptr)
// {
//     release();
//     mPtr = ptr;
//     if (ptr)
//         mControlBlock = new ControlBlock();
//     else
//         mControlBlock = nullptr;
// }
