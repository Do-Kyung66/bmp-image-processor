/**
 * @file Logger.cpp
 */

#include "Logger.h"
#include "Exceptions.h"

#include <ctime>
#include <iomanip>

namespace ip {

void Logger::open(const std::string& path) {
    m_file.open(path, std::ios::app);

    if (!m_file.is_open()) {
        throw ArgumentError("Cannot open log file: " + path);
    }
}

void Logger::log(const std::string& message) {
    if (!m_file.is_open()) {
        return;
    }

    const std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);

    m_file << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "] "
        << message << "\n";
    // 프로그램이 도중에 멈춰도 여기까지의 기록은 남도록 바로 파일에 쓴다.
    m_file.flush();
}

} // namespace ip