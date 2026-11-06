/**
 * @file HistogramFilter.cpp
 */

#include "HistogramFilter.h"
#include "PixelUtils.h"

#include <array>
#include <iostream>
#include <iomanip>
#include <string>

namespace ip {

namespace {
constexpr int BIN_COUNT = 16;
constexpr int BIN_SIZE = 16;
constexpr int MAX_BAR_LENGTH = 50;

} // anonymous namespace

void HistogramFilter::apply(ImageBuffer& image) const {

    std::array<int, BIN_COUNT> counts{};

    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);
        for (int x = 0; x < image.width(); ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            const int luminance = calculateLuminance(pixel);

            ++counts[luminance / BIN_SIZE];
        }
    }

    int maxCount = 0;
    for (const int count : counts) {
        if (maxCount < count) {
            maxCount = count;
        }
    }

    std::cout << "Luminance Histogram\n";
    for (int i = 0; i < BIN_COUNT; ++i) {
        const int rangeStart = i * BIN_SIZE;
        const int rangeEnd = rangeStart + BIN_SIZE - 1;
        const int barLength = static_cast<int>(
            static_cast<long long>(counts[i]) * MAX_BAR_LENGTH / maxCount);

        std::cout << std::setw(3) << rangeStart << " - "
            << std::setw(3) << rangeEnd << " | "
            << std::string(barLength, '#') << " "
            << counts[i] << "\n";
    }
}

std::string HistogramFilter::name() const {
    return "histogram";
}

} // namespace ip
