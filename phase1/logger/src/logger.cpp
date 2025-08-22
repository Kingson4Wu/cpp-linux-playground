#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

Logger::Logger() {
    m_logFile.open("app.log", std::ios::out | std::ios::app);
    m_workerThread = std::thread(&Logger::worker, this);
}

Logger::~Logger() {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_exit = true;
    }
    m_cv.notify_one();
    m_workerThread.join();
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

std::string levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:              return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " ";
    ss << "[" << levelToString(level) << "] ";
    ss << "[Thread " << std::this_thread::get_id() << "] ";
    ss << message;

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(ss.str());
    }
    m_cv.notify_one();
}

void Logger::worker() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return !m_queue.empty() || m_exit; });

        if (m_exit && m_queue.empty()) {
            break;
        }

        std::string message = m_queue.front();
        m_queue.pop();
        lock.unlock();

        if (m_logFile.is_open()) {
            m_logFile << message << std::endl;
        } else {
            std::cout << message << std::endl;
        }
    }
}
