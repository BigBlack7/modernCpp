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
    {
        // CRTP
        GetCommand cmd;
        cmd.execute();
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
    std::cout << "---------------------------------------------------" << std::endl;
    double node = j["pi"];
    std::cout << typeid(node).name() << std::endl;
    std::string n;
    j["name"].get_to(n);
    std::cout << n << std::endl;
    std::cout << j["pi"].get<double>() << ", " << j["name"].get<std::string>() << std::endl;
}

void useLRVals()
{
    std::cout << std::boolalpha;
    std::string str = "rendering";
    std::string &lr = str;
    std::string &&rr = "renderer";
    // if use decltype((str)), (str) will be considered as a lvalue expression
    // std::cout << "l_val: " << std::is_lvalue_reference<decltype(str)>::value << std::endl;
    // std::cout << "r_val: " << std::is_rvalue_reference<decltype("render")>::value << std::endl;
    // std::cout << "l_val_r: " << std::is_lvalue_reference<decltype(lr)>::value << std::endl;
    // std::cout << "r_val_r: " << std::is_rvalue_reference<decltype(rr)>::value << std::endl;
    // std::cout << "----------------" << std::endl;
    // check_reference(std::forward<std::string&&>(str));
    // std::cout << "----------------" << std::endl;
    // check_reference(std::move(rr));
    // std::cout << "----------------" << std::endl;
    // check_reference(2);
    Person p1 = create<Person>(str, 18);
    Person p2 = create<Person>("render", 23);
}

void useSingleton()
{
    std::mutex mtx;
    std::thread t1([&]()
                   {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "t1: " << Singleton2::getInstance() << std::endl; });

    std::thread t2([&]()
                   {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "t2: " << Singleton2::getInstance() << std::endl; });

    t1.join();
    t2.join();
}

void useIO()
{
    // write
    std::ofstream outfile("../../../outfile.txt");
    if (!outfile)
    {
        std::cerr << "open file failed" << std::endl;
        return;
    }
    outfile << "zmh.\n";
    outfile << "age: " << 18 << std::endl;
    outfile << "order: " << "NO.1" << std::endl;
    outfile.close();

    // read
    std::ifstream infile("../../../outfile.txt");
    if (!infile)
    {
        std::cerr << "open file failed" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(infile, line))
    {
        std::cout << line << std::endl;
    }
    infile.close();

    // add
    std::fstream addfile("../../../outfile.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!addfile)
    {
        std::cerr << "open file failed" << std::endl;
        return;
    }
    addfile << "GPA: A+\n";
    addfile.close();

    // ss
    std::stringstream ss;
    ss << "zmh, " << "good boy.";
    std::cout << ss.str() << std::endl;

    // os
    std::ostringstream os;
    os << "zmh, " << "good boy.";
    std::cout << os.str() << std::endl;
}

void useLogger()
{
    Logger logger("../../../log.txt");
    logger.log(LogLevel::Info, "zmh, {} years old, is NO.{}, use weapoon {}.", 18.5, 1, "world");
}

void useGLM()
{
    glm::vec3 normal(0, 1, 0);
    glm::vec3 view(-0.707107, 0.707107, 0);
    glm::vec3 light = -view + 2.f * glm::dot(view, normal) * normal;
    std::cout << "light: " << light.x << ", " << light.y << ", " << light.z << std::endl;
}

void useSpdlog()
{
    spdlog::set_level(spdlog::level::debug);
    SPDLOG_DEBUG("debug");
    SPDLOG_INFO("info");
    SPDLOG_WARN("warn");
    SPDLOG_ERROR("error");
}

void useThreadPool()
{
    ThreadPool pool;
    pool.setMode(ThreadMode::CACHE);
    pool.start(4);
    
    using uLong = unsigned long long;
    class Test : public Task
    {
    public:
        Test(int begin, int end) : mBegin(begin), mEnd(end) {}
        Any run() override
        {

            std::cout << "test begin:" << std::this_thread::get_id() << std::endl;
            uLong sum = 0;
            for (uLong i = mBegin; i <= mEnd; i++)
            {
                sum += i;
            }
            std::cout << "test end:" << std::this_thread::get_id() << std::endl;
            return sum;
        }
        int mBegin;
        int mEnd;
    };

    Result res1 = pool.submitTask(std::make_shared<Test>(1, 100000000));
    uLong sum1 = res1.get().cast<uLong>();

    Result res2 = pool.submitTask(std::make_shared<Test>(100000001, 200000000));
    uLong sum2 = res2.get().cast<uLong>();

    std::cout << "sum1: " << sum1 << std::endl;
    std::cout << "sum2: " << sum2 << std::endl;
    std::cout << "sum3: " << sum1 + sum2 << std::endl;

    uLong sum = 0;
    for (uLong i = 1; i <= 200000000; i++)
    {
        sum += i;
    }
    std::cout << "sum: " << sum << std::endl;
}

int main()
{
    useThreadPool();

    std::cout << "----------------Hello, Cpp!----------------" << std::endl;
    return 0;
}