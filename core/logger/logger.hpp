#pragma once
#include "helper.hpp"

class logQueue
{
private:
    std::queue<std::string> mQueue;
    std::mutex mMutex;
    std::condition_variable mConVar;
    bool mShutdown{false};

public:
    void push(const std::string &msg)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mQueue.push(msg);
        mConVar.notify_one();
    }

    bool pop(std::string &msg)
    {
        std::unique_lock<std::mutex> lock(mMutex);

        mConVar.wait(lock, [this]
                     { return !mQueue.empty() || mShutdown; });

        if (mShutdown && mQueue.empty())
        {
            return false;
        }
        msg = mQueue.front();
        mQueue.pop();
        return true;
    }

    void shutdown()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mShutdown = true;
        mConVar.notify_all();
    }
};

enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};

class Logger
{
private:
    logQueue mLogQueue;
    std::thread mWorkerThread;
    std::ofstream mLogFile;
    std::atomic<bool> mExit;

private:
    template <typename... Args>
    std::string formatMessage(const std::string &format, Args &&...args)
    {
        std::vector<std::string> argStrings = {toStringHelper(std::forward<Args>(args))...};
        std::ostringstream oss;
        size_t argIndex = 0;
        size_t pos = 0;
        size_t placeholder = format.find("{}", pos);
        while (placeholder != std::string::npos)
        {
            oss << format.substr(pos, placeholder - pos);
            if (argIndex < argStrings.size())
            {
                oss << argStrings[argIndex++];
            }
            else
            {
                oss << "{}";
            }

            pos = placeholder + 2;
            placeholder = format.find("{}", pos);
        }

        oss << format.substr(pos);
        while (argIndex < argStrings.size())
        {
            oss << argStrings[argIndex++];
        }
        return "[" + getCurrnetTime() + "] " + oss.str();
    }

public:
    Logger(const std::string &filename) : mLogFile(filename, std::ios::out | std::ios::app), mExit(false)
    {
        if (!mLogFile.is_open())
        {
            throw std::runtime_error("Failed to open log file");
        }

        mWorkerThread = std::thread([this]()
                                    {
                                        std::string msg;
                                        while (mLogQueue.pop(msg))
                                        {
                                            mLogFile << msg << std::endl;
                                        } });
    }

    ~Logger()
    {
        mExit = true;
        mLogQueue.shutdown();
        if (mWorkerThread.joinable())
        {
            mWorkerThread.join();
        }

        if (mLogFile.is_open())
        {
            mLogFile.close();
        }
    }

    template <typename... Args>
    void log(LogLevel level, const std::string &format, Args &&...args)
    {
        std::string levelStr;
        switch (level)
        {
        case LogLevel::Debug:
            levelStr = "[DEBUG] ";
            break;
        case LogLevel::Info:
            levelStr = "[INFO] ";
            break;
        case LogLevel::Warning:
            levelStr = "[WARNING] ";
            break;
        case LogLevel::Error:
            levelStr = "[ERROR] ";
            break;
        default:
            levelStr = "[UNKNOWN] ";
            break;
        }
        mLogQueue.push(levelStr + formatMessage(format, std::forward<Args>(args)...));
    }
};
