#pragma once
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <concepts>

template <typename T, typename U>
class Pair
{
public:
    Pair(T first, U second) : first(first), second(second) {};
    void print()
    {
        std::cout << first << " " << second << std::endl;
    }

private:
    T first;
    U second;
};
template <typename T, typename U>
class Pair<T *, U *>
{
public:
    Pair(T first, U *second) : first(first), second(second) {};
    void print()
    {
        std::cout << *first << " " << *second << std::endl;
    }

private:
    T first;
    U *second;
};
template <typename U>
class Pair<std::string, U *>
{
public:
    Pair(std::string first, U *second) : first(first), second(second) {};
    void print()
    {
        std::cout << first << " " << *second << std::endl;
    }

private:
    std::string first;
    U *second;
};

void printAll()
{
    std::cout << std::endl;
}
template <typename T, typename... Args>
void printAll(T first, Args... args)
{
    std::cout << first << " ";
    printAll(args...);
}

// template fold
template <typename... Args>
void printAll17(Args... args)
{
    ((std::cout << args << " "), ...);
}

template <typename... Args>
auto sumArgs(Args... args) -> decltype((args + ...))
{
    return (args + ...);
}

// condition compile -> SFINAE
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type printInt(T t)
{
    std::cout << t << std::endl;
}

template <typename T>
concept Integral = std::is_integral_v<T>;
void printTypeIntegral(Integral auto t)
{
    std::cout << typeid(t).name() << "->" << t << std::endl;
}

template <typename T>
class has_foo
{
private:
    typedef char yes[1];
    typedef char no[2];
    template <typename U, void (U::*)()>
    struct SFINAE
    {
    };

    template <typename U>
    static yes &test(SFINAE<U, &U::foo> *);

    template <typename U>
    static no &test(...);

public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename T>
typename std::enable_if<has_foo<T>::value, void>::type call_foo(T &obj)
{
    obj.foo();
    std::cout << "call_foo" << std::endl;
}

class WithFoo
{
public:
    void foo()
    {
        std::cout << "WithFoo::foo" << std::endl;
    }
};

class WithoutFoo
{
};

// type extract
template <typename T, typename = void>
struct has_non_void_value_type:std::false_type
{
};
template <typename T>
struct has_non_void_value_type<T,std::enable_if_t<!std::is_void_v<typename T::value_type>>>:std::true_type
{
};
template <typename T, bool hasVal = has_non_void_value_type<T>::value>
struct TypePrinter;

template <typename T>
struct TypePrinter<T, true>
{
    static void print()
    {
        std::cout << "TypePrinter has type -> " << typeid(T).name() << std::endl;
    }
};

template <typename T>
struct TypePrinter<T, false>
{
    static void print()
    {
        std::cout << "TypePrinter has no type -> " << typeid(T).name() << std::endl;
    }
};

struct WithValue
{
    using value_type = int;
};

struct WithoutValue
{
};
