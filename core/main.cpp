#include <iostream>
#include <string>
#include "ptr/zSharedPtr.hpp"

int main()
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

    std::cout << "Hello World!\n";
    return 0;
}