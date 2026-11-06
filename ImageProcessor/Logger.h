#pragma once

/**
 * @file Logger.h
 * @brief 실행 기록을 파일에 남기는 클래스.
 */

#include <fstream>
#include <string>

namespace ip {

/**
 * @brief 시간과 메시지를 한 줄씩 파일에 기록한다. 파일을 열지 않으면 기록하지 않는다.
 */
class Logger {
public:
    /**
     * @param path 로그 파일 경로. 기존 내용 뒤에 이어서 쓴다.
     * @throws ArgumentError 파일을 열 수 없는 경우.
     */
    void open(const std::string& path);

    /// 현재 시간과 함께 메시지를 한 줄 기록한다.
    void log(const std::string& message);

private:
    std::ofstream m_file;
};

} // namespace ip