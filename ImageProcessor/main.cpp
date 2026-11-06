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
#include <memory>
#include <vector>

 // TODO: 본인이 구현한 필터 헤더를 include 하세요.
 // #include "GrayscaleFilter.h"
 // #include "ThresholdFilter.h"
 // ...

// 필터 헤더는 FilterFactory.cpp에서 include한다.
#include "FilterFactory.h"

#include "Logger.h"

#include <string>
#include <chrono>


int main(int argc, char* argv[]) {
    ip::Logger logger;

    try {
        // ── CLI 인자 파싱 (제공된 코드) ─────────────────────────
        const ip::ProgramOptions options = ip::CommandLineParser::parse(argc, argv);

        // 로그 파일 열기
        if (!options.logPath.empty()) {
            logger.open(options.logPath);
        }

        // ── BMP 로드 (제공된 코드) ──────────────────────────────
        ip::ImageBuffer image = ip::BmpParser::loadFromFile(options.inputPath);
        std::cout << "Loaded: " << image.width() << " x " << image.height() << "\n";

        // 불러온 파일 경로와 이미지 사이즈를 로그파일에 적는다. 
        logger.log("Loaded: " + options.inputPath + " (" + std::to_string(image.width())
            + " x " + std::to_string(image.height()) + ")");

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

        // 필터 생성은 FilterFactory.cpp의 createFilter()로 옮겼다.
        // 새 필터를 추가할 때는 main을 고치지 않고 createFilter()에만 분기를 추가한다.
        std::vector<std::unique_ptr<ip::FilterBase>> filters;
        if (!options.pipeline.empty()) {
            filters = ip::createPipeline(options.pipeline, options.threadCount);
        }
        else {
            filters.push_back(ip::createFilter(options.filterName, options.threadCount));
        }

        // 필터마다 적용에 걸린 시간을 재서 로그에 남긴다.
        for (const std::unique_ptr<ip::FilterBase>& filter : filters) {
            const auto start = std::chrono::steady_clock::now();
            filter->apply(image);
            const auto end = std::chrono::steady_clock::now();

            const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            logger.log("Applied: " + filter->name() + " (" + std::to_string(elapsed.count()) + " ms)");
        }

        // ── BMP 저장 (제공된 코드) ──────────────────────────────
        ip::BmpParser::saveToFile(options.outputPath, image);
        std::cout << "Saved:  " << options.outputPath << "\n";
        
        logger.log("Saved: " + options.outputPath);

        return 0;
    }
    catch (const ip::ArgumentError& e) {
        logger.log(std::string("Error: ") + e.what());
        std::cerr << e.what() << "\n\n";
        ip::CommandLineParser::printUsage(argc > 0 ? argv[0] : "ImageProcessor");
        return 4;
    }
    catch (const ip::BmpParseError& e) {
        logger.log(std::string("Error: ") + e.what());
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (const ip::FilterError& e) {
        logger.log(std::string("Error: ") + e.what());
        std::cerr << e.what() << std::endl;
        return 3;
    }
    catch (const std::exception& e) {
        logger.log(std::string("Unexpected error: ") + e.what());
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
}
