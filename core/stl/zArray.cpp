#include "zArray.hpp"

ZArray::ZArray(size_t initial_size) : mSize(initial_size), mData(new int[initial_size])
{
    if (mData == nullptr)
    {
        throw std::bad_alloc();
    }
}

ZArray::~ZArray()
{
    if (mData != nullptr)
    {
        delete[] mData;
    }
}

void ZArray::resize(size_t new_size)
{
    int *temp = static_cast<int *>(realloc(mData, new_size * sizeof(int)));
    for (size_t i = mSize; i < new_size; ++i)
        temp[i] = 0;
    if (temp == nullptr)
    {
        throw std::bad_alloc();
    }
    mData = temp;
    mSize = new_size;
}

void ZArray::print() const
{
    for (size_t i = 0; i < mSize; ++i)
    {
        std::cout << mData[i] << " ";
    }
    std::cout << std::endl;
}
