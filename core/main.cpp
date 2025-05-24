#include "core.hpp"
#include "ptr/zSharedPtr.hpp"
#include "stl/zArray.hpp"
#include "memory/zMemoryPool.hpp"

void useZSharedPtr()
{
    ZSharedPtr<std::string> ptr1;
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    ZSharedPtr<std::string> ptr2(new std::string("hello"));
    ZSharedPtr<std::string> ptr3(ptr2);
    std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;
    ZSharedPtr<std::string> ptr4(std::move(ptr2));
    std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;
    std::cout << "ptr4 use_count: " << ptr4.use_count() << std::endl;
}

void useZArray()
{
    ZArray arr(3);
    for (int i = 0; i < arr.size(); ++i)
        arr[i] = i * 2;
    arr.print();
    arr.resize(5);
    arr[3] = 10;
    arr[4] = 11;
    arr.print();
}

void useZMemoryPool()
{
    ZMemoryPool pool(sizeof(std::string), 3);
    void *ptr1 = pool.allocate();
    void *ptr2 = pool.allocate();

    std::string *str1 = new (ptr1) std::string("hello");
    std::string *str2 = new (ptr2) std::string("world");

    std::cout << *str1 << std::endl;
    std::cout << *str2 << std::endl;

    str1->~basic_string();
    str2->~basic_string();

    pool.deallocate(ptr1);
    pool.deallocate(ptr2);
}

int main()
{
    useZSharedPtr();
    std::cout << "Hello Cpp!" << std::endl;
    return 0;
}