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

void useZAVLMap()
{
    ZAVLMap<int, std::string> map;
    map.put(150, "apple");
    map.put(200, "peach");
    map.put(160, "grape");
    std::vector<std::pair<int, std::string>> res = map.inorderTraversal();
    for (auto &p : res)
    {
        std::cout << p.first << "=>" << p.second << std::endl;
    }
    map.erase(160);
    std::vector<std::pair<int, std::string>> res1 = map.inorderTraversal();
    for (auto &p : res1)
    {
        std::cout << p.first << "=>" << p.second << std::endl;
    }
}

void useZHashMap()
{
    ZHashMap<std::string, int> map;
    map.insert("apple", 3);
    map.insert("peach", 4);
    map.insert("orange", 5);
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        std::cout << it->first << "=>" << it->second << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
    map.erase("orange");
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        std::cout << it->first << "=>" << it->second << std::endl;
    }
}

void useTemplate()
{
    {
        // printTypeIntegral(1);
        // WithFoo wf;
        // call_foo(wf);
        // call_foo11(wf);
        // call_foo20(wf);
        // // WithoutFoo wof;
        // // call_foo(wof); error
        // // call_fooM(wof); error
        // // call_foo20(wof); error
        // TypePrinter<WithValue>::print();
        // TypePrinter<WithoutValue>::print();
    }
    {
        std::cout << "Factorial: " << Factorial<5>::value << std::endl;
        std::cout << "&Factorial:" << &Factorial<5>::value << std::endl;
        std::cout << "Variadic Templates: " << sum<1, 2, 3>::value << std::endl;
        static_assert(is_addable<int>::value, "");
        if (is_addable<int>::value)
            std::cout << "int should be addable" << std::endl;
        if (!is_addable<void *>::value)
            std::cout << "void* should not be addable" << std::endl;
        using list = TypeList<int, double, char>;
        using third_type = typeAt<list, 2>::type;
        std::cout << "third_type: " << typeid(third_type).name() << std::endl;
    }
}

void useJson()
{
    using Json = nlohmann::json;
    Json j = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {{"everything", 42}}},
        {"list", {1, 0, 2}},
        {"object", {{"currency", "USD"}, {"value", 42.99}}}};
    std::cout << "j: " << j.dump() << std::endl;

    Json oArray = {1, 2, 3};
    Json oInt = 1;
    Json oString = "hello";

    // _json
    using namespace nlohmann::literals;
    std::string str = R"({"name":"Niels","list":[1,2,3],"object":{"currency":"USD","value":42.99}})";
    Json str_ = R"({"name":"Niels","list":[1,2,3],"object":{"currency":"USD","value":42.99}})"_json;
    std::cout << "str_: " << str_.dump() << std::endl;
    std::cout << "str: " << Json::parse(str) << std::endl; // parse(str, nullptr, true, true) 1.para 2.callback 3.message analyze 4.comments ignore
}

int main()
{
    useJson();

    std::cout << "----Hello, Cpp!----" << std::endl;
    return 0;
}