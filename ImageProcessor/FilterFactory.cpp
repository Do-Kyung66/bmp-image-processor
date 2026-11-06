/**
 * @file FilterFactory.cpp
 */

#include "FilterFactory.h"
#include "Exceptions.h"

#include "BrightnessFilter.h"
#include "ContrastFilter.h"
#include "ConvolutionFilter.h"
#include "CropFilter.h"
#include "FlipFilter.h"
#include "GrayscaleFilter.h"
#include "HistogramFilter.h"
#include "ResizeFilter.h"
#include "ThresholdFilter.h"

#include <stdexcept>
#include <vector>

namespace ip {

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
            throw FilterError("Invalid filter value: " + valueText);
        }
        return value;
    }
    catch (const std::logic_error&) {
        throw FilterError("Invalid filter value: " + valueText);
    }
}

/**
 * @brief "crop:10:20:100:50" 형식의 필터 이름에서 값을 모두 꺼낸다.
 * @throws FilterError 값이 정수가 아닌 경우.
 */
std::vector<int> parseIntValues(const std::string& filterName) {
    std::vector<int> values;
    std::size_t start = filterName.find(':');

    while (start != std::string::npos) {
        const std::size_t end = filterName.find(':', start + 1);
        values.push_back(parseIntValue(filterName.substr(start, end - start)));
        start = end;
    }
    return values;
}

double parseDoubleValue(const std::string& filterName) {
    const std::string valueText = filterName.substr(filterName.find(':') + 1);

    try {
        std::size_t parsedLength = 0;
        const double value = std::stod(valueText, &parsedLength);

        if (parsedLength != valueText.size()) {
            throw FilterError("Invalid filter value: " + valueText);
        }
        return value;
    }
    catch (const std::logic_error&) {
        throw FilterError("Invalid filter value: " + valueText);
    }
}

/// 문자열 앞뒤의 공백을 지운다.
std::string trim(const std::string& text) {
    const std::size_t first = text.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(" \t");
    return text.substr(first, last - first + 1);
}
} // anonymous namespace


std::unique_ptr<FilterBase> createFilter(const std::string& filterName, int threadCount) {
    // 인자가 없는 필터
    if (filterName == "grayscale") {
        return std::make_unique<GrayscaleFilter>();
    }
    if (filterName == "histogram") {
        return std::make_unique<HistogramFilter>();
    }

    // 값이 하나인 필터
    if (filterName.rfind("threshold:", 0) == 0) {
        return std::make_unique<ThresholdFilter>(parseIntValue(filterName));
    }
    if (filterName.rfind("brightness:", 0) == 0) {
        return std::make_unique<BrightnessFilter>(parseIntValue(filterName));
    }
    if (filterName.rfind("contrast:", 0) == 0) {
        return std::make_unique<ContrastFilter>(parseDoubleValue(filterName));
    }

    // 종류를 고르는 필터
    if (filterName == "flip:horizontal") {
        return std::make_unique<FlipFilter>(FlipFilter::Direction::HORIZONTAL);
    }
    if (filterName == "flip:vertical") {
        return std::make_unique<FlipFilter>(FlipFilter::Direction::VERTICAL);
    }
    if (filterName == "blur") {
        return std::make_unique<ConvolutionFilter>(ConvolutionFilter::KernelType::BLUR, threadCount);
    }
    if (filterName == "sharpen") {
        return std::make_unique<ConvolutionFilter>(ConvolutionFilter::KernelType::SHARPEN, threadCount);
    }

    // 값이 여러 개인 필터
    if (filterName.rfind("crop:", 0) == 0) {
        const std::vector<int> values = parseIntValues(filterName);
        if (values.size() != 4) {
            throw FilterError("Crop needs x:y:width:height: " + filterName);
        }
        return std::make_unique<CropFilter>(values[0], values[1], values[2], values[3]);
    }
    if (filterName.rfind("resize:", 0) == 0) {
        const std::vector<int> values = parseIntValues(filterName);
        if (values.size() != 2) {
            throw FilterError("Resize needs width:height: " + filterName);
        }
        return std::make_unique<ResizeFilter>(values[0], values[1]);
    }

    throw FilterError("Unknown filter: " + filterName);
}

std::vector<std::unique_ptr<FilterBase>> createPipeline(const std::string& pipeline, int threadCount) {
    std::vector<std::unique_ptr<FilterBase>> filters;
    std::size_t start = 0;

    while (true) {
        const std::size_t comma = pipeline.find(',', start);
        const std::string name = trim(pipeline.substr(start, comma - start));

        if (name.empty()) {
            throw FilterError("Empty filter name in pipeline: " + pipeline);
        }
        filters.push_back(createFilter(name, threadCount));

        if (comma == std::string::npos) {
            break;
        }
        start = comma + 1;
    }
    return filters;
}

} // namespace ip