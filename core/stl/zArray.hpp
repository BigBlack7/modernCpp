#pragma once
#include <iostream>

class ZArray
{
private:
    size_t mSize{0};
    int *mData{nullptr};

public:
    ZArray(size_t initial_size = 1);
    ~ZArray();

    void resize(size_t new_size);
    void print() const;

    int &operator[](size_t index) { return mData[index]; }

    size_t size() const { return mSize; }
};