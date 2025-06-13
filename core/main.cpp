#include "core.hpp"

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

void printList(const std::list<int> &lst)
{
    for (const auto &elem : lst)
        std::cout << elem << " ";
    std::cout << std::endl;
}
void useZList()
{
    std::list<int> stdList{7, 10};
    auto stdIt = stdList.begin();
    stdList.insert(stdIt, 3);
    stdList.erase(++stdIt);
    printList(stdList);
    std::cout << "-----------------------------------" << std::endl;
    ZList<int> myList;
    myList.push_back(10);
    myList.push_front(5);
    myList.push_back(15);
    myList.insert(++myList.begin(), 7);
    myList.print(); // 5 7 10 15

    myList.pop_front();
    myList.pop_back();
    myList.print(); // 7 10

    auto it = myList.begin();
    myList.insert(it, 3);
    myList.erase(++it);
    myList.print(); // 3 7

    myList.remove(7);
    myList.print(); // 3

    myList.clear();
    myList.print(); // empty
}

void useZDeque()
{
    ZDeque<std::string> myDeque;
    myDeque.push_back("Hello");
    myDeque.push_back("World");
    myDeque.print(); // Hello World

    myDeque.push_front("C++");
    myDeque.push_front("Programming");
    myDeque.print(); // Programming C++ Hello World

    myDeque.pop_front();
    myDeque.print(); // C++ Hello World

    myDeque.pop_back();
    myDeque.print(); // C++ Hello
}

void useZBSTMap()
{
    ZBSTMap<int, std::string> map;
    map.insert(150, "apple");
    map.insert(130, "banana");
    map.insert(170, "orange");
    map.insert(165, "peach");
    map.print();
    std::cout << "-----------------------------------" << std::endl;
    map.erase(130);
    map.print();
    std::cout << "-----------------------------------" << std::endl;
    map[150] = "banana";
    map.print();
}

int main()
{
    useZBSTMap();
    std::cout << "----Hello, Cpp!----" << std::endl;
    return 0;
}