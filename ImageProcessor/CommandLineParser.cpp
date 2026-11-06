/**
 * @file CommandLineParser.cpp
 */

#include "CommandLineParser.h"
#include "Exceptions.h"

#include <iostream>
#include <string>
#include <stdexcept>

namespace ip {

namespace {
    /// argv 의 다음 인자를 안전하게 가져온다.
    std::string nextArg(int argc, char* argv[], int& i, const std::string& flag) {
        if (i + 1 >= argc) {
            throw ArgumentError(flag + ": missing value");
        }
        return argv[++i];
    }

    constexpr int MAX_THREAD_COUNT = 64;

    /**
     * @brief "--threads" 값을 정수로 바꾼다.
     * @throws ArgumentError 정수가 아니거나 1~MAX_THREAD_COUNT 밖인 경우.
     */
    int parseThreadCount(const std::string& text) {
        int value = 0;
        try {
            std::size_t parsedLength = 0;
            value = std::stoi(text, &parsedLength);

            if (parsedLength != text.size()) {
                throw ArgumentError("--threads must be a number: " + text);
            }
        }
        catch (const std::logic_error&) {
            throw ArgumentError("--threads must be a number: " + text);
        }

        if (value < 1 || value > MAX_THREAD_COUNT) {
            throw ArgumentError("--threads must be 1-" + std::to_string(MAX_THREAD_COUNT) + ": " + text);
        }
        return value;
    }
} // anonymous namespace

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {
    ProgramOptions options;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--input" || arg == "-i") {
            options.inputPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--output" || arg == "-o") {
            options.outputPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--filter" || arg == "-f") {
            options.filterName = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--pipeline" || arg == "-p") {
            options.pipeline = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--log" || arg == "-l") {
            options.logPath = nextArg(argc, argv, i, arg);
        }
        else if (arg == "--threads" || arg == "-t") {
            options.threadCount = parseThreadCount(nextArg(argc, argv, i, arg));
        }
        else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            std::exit(0);
        }
        else {
            throw ArgumentError("Unknown option: " + arg);
        }
    }

    // 필수 인자 검증
    if (options.inputPath.empty()) {
        throw ArgumentError("--input is required");
    }
    if (options.outputPath.empty()) {
        throw ArgumentError("--output is required");
    }
    if (options.filterName.empty() && options.pipeline.empty()) {
        throw ArgumentError("--filter or --pipeline is required");
    }
    if (!options.filterName.empty() && !options.pipeline.empty()) {
        throw ArgumentError("Use either --filter or --pipeline, not both");
    }

    return options;
}

void CommandLineParser::printUsage(const std::string& exeName) {
    std::cout
        << "Usage:\n"
        << "  " << exeName << " --input <path> --output <path> --filter <name>\n\n"
        << "Options:\n"
        << "  -i, --input   <path>   Input BMP file (24-bit, uncompressed)\n"
        << "  -o, --output  <path>   Output BMP file\n"
        << "  -f, --filter  <name>   Filter to apply (e.g. grayscale, threshold:128)\n"
        << "  -p, --pipeline <list>  Filters to apply in order (e.g. \"grayscale, blur\")\n"
        << "  -l, --log     <path>   Append progress to a log file (optional)\n"
        << "  -t, --threads <count>  Threads for blur/sharpen (1-64, default 1)\n"
        << "  -h, --help             Show this message\n\n"
        << "Examples:\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f grayscale\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f threshold:128\n";
}

} // namespace ip
