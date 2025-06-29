#include "singleton.hpp"
Hungry *Hungry::mPtr = Hungry::getInstance();
Lazy *Lazy::mPtr = nullptr;
std::mutex Lazy::mLock;
std::shared_ptr<Singleton2> Singleton2::mPtr = nullptr;
