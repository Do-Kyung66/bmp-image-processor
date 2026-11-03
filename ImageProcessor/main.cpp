/**
 * @file main.cpp
 * @brief ImageProcessor 진입점 — 지원자가 작성해야 할 파일입니다.
 *
 * BMP 입출력과 커맨드라인 파싱은 제공된 코드가 처리합니다.
 * 본 과제에서 작성해야 할 것은 단 하나입니다:
 *
 *     ▶ 이미지 처리 필터 2개 이상 구현 + main 의 TODO 위치에 연결
 *
 * 또한 일관된 컨벤션과 예외 처리, 메모리 안정성도 함께 평가됩니다.
 */

#include "BmpParser.h"
#include "CommandLineParser.h"
#include "ImageBuffer.h"
#include "Exceptions.h"

#include <iostream>

 // TODO: 본인이 구현한 필터 헤더를 include 하세요.
 // #include "GrayscaleFilter.h"
 // #include "ThresholdFilter.h"
 // ...

#include "GrayscaleFilter.h"
#include "ThresholdFilter.h"
#include "BrightnessFilter.h"
#include "ContrastFilter.h"


#include <stdexcept>
#include <string>

namespace {

/**
* @brief "threshold:128" 형식의 필터 이름에서 값을 꺼낸다.
* @throws ip::FilterError 값이 정수가 아닌 경우.
*/
int parseIntValue(const std::string& filterName) {
    const std::string valueText = filterName.substr(filterName.find(':') + 1);

    try {
        std::size_t parsedLength = 0;
        const int value = std::stoi(valueText, &parsedLength);

        if (parsedLength != valueText.size()) {
            throw ip::FilterError("Invalid filter value: " + valueText);
        }
        return value;
    }
    catch (const std::logic_error&) {
        throw ip::FilterError("Invalid filter value: " + valueText);
    }
}

double parseDoubleValue(const std::string& filterName) {
    const std::string valueText = filterName.substr(filterName.find(':') + 1);

    try {
        std::size_t parsedLength = 0;
        const double value = std::stod(valueText, &parsedLength);

        if (parsedLength != valueText.size()) {
            throw ip::FilterError("Invalid filter value: " + valueText);
        }
        return value;
    }
    catch (const std::logic_error&) {
        throw ip::FilterError("Invalid filter value: " + valueText);
    }
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    try {
        // ── CLI 인자 파싱 (제공된 코드) ─────────────────────────
        const ip::ProgramOptions options = ip::CommandLineParser::parse(argc, argv);

        // ── BMP 로드 (제공된 코드) ──────────────────────────────
        ip::ImageBuffer image = ip::BmpParser::loadFromFile(options.inputPath);
        std::cout << "Loaded: " << image.width() << " x " << image.height() << "\n";

        // ───────────────────────────────────────────────────────
        // TODO: options.filterName 에 따라 적절한 필터를 생성하고
        //       image 에 적용하세요.
        //
        //   예시 코드 (참고용):
        //
        //     if (options.filterName == "grayscale") {
        //         GrayscaleFilter filter;
        //         filter.apply(image);
        //     }
        //     else if (options.filterName == "threshold:128") {
        //         ThresholdFilter filter(128);
        //         filter.apply(image);
        //     }
        //     else {
        //         throw ip::FilterError("Unknown filter: " + options.filterName);
        //     }
        //
        //   ※ 가산점 항목:
        //     - 추상 클래스(FilterBase) 기반 다형성 설계
        //     - 필터 파이프라인 체인 (CLI 옵션 확장 필요)
        //     - 멀티쓰레드 처리
        //     - 로그 파일 출력 (CLI 옵션 확장 필요)
        // ───────────────────────────────────────────────────────

        // ↓ 여기에 필터 적용 코드를 작성하세요.
        if (options.filterName == "grayscale") {
            ip::GrayscaleFilter filter;
            filter.apply(image);
        }
        else if (options.filterName.rfind("threshold:", 0) == 0) {
            ip::ThresholdFilter filter(parseIntValue(options.filterName));
            filter.apply(image);
        }
        else if (options.filterName.rfind("brightness:", 0) == 0) { 
            ip::BrightnessFilter filter(parseIntValue(options.filterName));
            filter.apply(image);
        }
        else if (options.filterName.rfind("contrast:", 0) == 0) { 
            ip::ContrastFilter filter(parseDoubleValue(options.filterName));
            filter.apply(image);
        }
        else {
            throw ip::FilterError("Unknown filter: " + options.filterName);
        }

        // ── BMP 저장 (제공된 코드) ──────────────────────────────
        ip::BmpParser::saveToFile(options.outputPath, image);
        std::cout << "Saved:  " << options.outputPath << "\n";
        return 0;
    }
    catch (const ip::ArgumentError& e) {
        std::cerr << e.what() << "\n\n";
        ip::CommandLineParser::printUsage(argc > 0 ? argv[0] : "ImageProcessor");
        return 4;
    }
    catch (const ip::BmpParseError& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (const ip::FilterError& e) {
        std::cerr << e.what() << std::endl;
        return 3;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
}
