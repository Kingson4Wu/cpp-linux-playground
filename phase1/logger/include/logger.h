#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <fstream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();
    void log(LogLevel level, const std::string& message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void worker();

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<std::string> m_queue;
    std::thread m_workerThread;
    bool m_exit = false;
    std::ofstream m_logFile;
};

#endif // LOGGER_H
