#pragma once
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <concepts>

// normal
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

// partial specialization
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

// full specialization
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

// template variadic
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

// modern cpp implement up
template <typename T>
concept Integral = std::is_integral_v<T>;
void printTypeIntegral(Integral auto t)
{
    std::cout << typeid(t).name() << "->" << t << std::endl;
}

// type extract
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

// c++ 11
template <typename, typename = std::void_t<>>
struct has_foo11 : std::false_type
{
};
template <typename T>
struct has_foo11<T, std::void_t<decltype(std::declval<T>().foo())>> : std::true_type
{
};

template <typename T>
typename std::enable_if<has_foo11<T>::value, void>::type call_foo11(T &obj)
{
    obj.foo();
    std::cout << "call_foo11" << std::endl;
}

// c++ 20
template <typename T>
concept HasFoo = requires(T t) {
    { t.foo() } -> std::same_as<void>;
};

template <HasFoo T>
void call_foo20(T &obj)
{
    obj.foo();
    std::cout << "call_foo20" << std::endl;
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
struct has_non_void_value_type : std::false_type
{
};
template <typename T>
struct has_non_void_value_type<T, std::enable_if_t<!std::is_void_v<typename T::value_type>>> : std::true_type
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
//----------------------------------------------
template <int N>
struct Factorial
{
    inline static const int value = N * Factorial<N - 1>::value; // c++ 17 inline
};
template <>
struct Factorial<0>
{
    inline static const int value = 1;
};

// if get the adress of value, and use 'inline',we dont need definition below
// template<int N>
// const int Factorial<N>::value;

// Variadic Templates
template <int... ns>
struct sum;

template <>
struct sum<>
{
    static const int value = 0;
};

template <int n, int... ns>
struct sum<n, ns...>
{
    static const int value = n + sum<ns...>::value;
};

// type addable
template <typename T, typename = void>
struct is_addable : std::false_type
{
};

template <typename T>
struct is_addable<T, decltype(void(std::declval<T>() + std::declval<T>()))> : std::true_type
{
};

// type trap
template <typename... Ts>
struct TypeList
{
};
template <typename List, std::size_t N>
struct typeAt;

template <typename Head, typename... Tail>
struct typeAt<TypeList<Head, Tail...>, 0>
{
    using type = Head;
};
template <typename Head, typename... Tail, std::size_t N>
struct typeAt<TypeList<Head, Tail...>, N>
{
    using type = typename typeAt<TypeList<Tail...>, N - 1>::type;
};

// forwarding reference
template <typename T>
void check_reference(T &&x)
{
    // T normal    -> T&& rvr
    // T reference -> T&& lvr
    std::cout << std::boolalpha;
    std::cout << "T is lval_r: " << std::is_lvalue_reference<T>::value << std::endl;
    std::cout << "T is rval_r: " << std::is_rvalue_reference<T>::value << std::endl;
    std::cout << "T is rval  : " << std::is_rvalue_reference<T &&>::value << std::endl;
    std::cout << "x is lval_r: " << std::is_lvalue_reference<decltype(x)>::value << std::endl;
    std::cout << "x is rval_r: " << std::is_rvalue_reference<decltype(x)>::value << std::endl;
}

class Person
{
public:
    std::string name;
    int age;

    Person(const std::string &n, int a) : name(n), age(a)
    {
        std::cout << "Constructed Person(const std::string&, int)" << std::endl;
    }

    Person(std::string &&n, int a) : name(std::move(n)), age(a)
    {
        std::cout << "Constructed Person(std::string&&, int)" << std::endl;
    }
};

template <typename T, typename... Args>
T create(Args &&...args)
{
    return T(std::forward<Args>(args)...);
}

// CRTP
template <typename Derived>
class Command
{
public:
    void execute()
    {
        static_cast<Derived *>(this)->executeImpl();
    }
};

class GetCommand : public Command<GetCommand>
{
public:
    void executeImpl()
    {
        std::cout << "GetCommand::executeImpl()" << std::endl;
    }
};