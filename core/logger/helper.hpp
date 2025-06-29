#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <atomic>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iomanip>

template <typename T>
std::string toStringHelper(T &&value)
{
    std::ostringstream oss;
    oss << std::forward<T>(value);
    return oss.str();
}

inline std::string getCurrnetTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    errno_t err = localtime_s(&localTime, &time);
    if (err != 0) {
        return "Error getting time";
    }
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}