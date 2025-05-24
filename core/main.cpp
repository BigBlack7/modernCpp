#include <iostream>
#include <string>
#include "ptr/zSharedPtr.hpp"
#include "stl/zArray.hpp"

int main()
{
#pragma region zSharedPtr
    // {
    //     ZSharedPtr<std::string> ptr1;
    //     std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    //     ZSharedPtr<std::string> ptr2(new std::string("hello"));
    //     ZSharedPtr<std::string> ptr3(ptr2);
    //     std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    //     std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;
    //     ZSharedPtr<std::string> ptr4(std::move(ptr2));
    //     std::cout << "ptr2 use_count: " << ptr2.use_count() << std::endl;
    //     std::cout << "ptr3 use_count: " << ptr3.use_count() << std::endl;
    //     std::cout << "ptr4 use_count: " << ptr4.use_count() << std::endl;
    // }
#pragma endregion

#pragma region zArray
    {
        // ZArray arr(3);
        // for (int i = 0; i < arr.size(); ++i)
        //     arr[i] = i * 2;
        // arr.print();
        // arr.resize(5);
        // arr[3] = 10;
        // arr[4] = 11;
        // arr.print();
    }
#pragma endregion

    std::cout << "Hello World!\n";
    return 0;
}