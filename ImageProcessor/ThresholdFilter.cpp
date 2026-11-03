/**
 * @file ThresholdFilter.cpp
 */

#include "ThresholdFilter.h"
#include "Exceptions.h"

namespace ip {

namespace {

// RGB 가중치 공식
// 0.299 × R + 0.587 × G + 0.114 × B
constexpr int BLUE_WEIGHT = 114;
constexpr int GREEN_WEIGHT = 587;
constexpr int RED_WEIGHT = 299;
constexpr int WEIGHT_SUM = 1000;

} // anonymous namespace

ThresholdFilter::ThresholdFilter(int threshold) : m_threshold(threshold) {
    if (threshold < 0 || threshold > 255) {
        throw FilterError("Threshold must be 0-255: " + std::to_string(threshold));
    }
}

void ThresholdFilter::apply(ImageBuffer& image) const {
    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < image.width(); ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            const int luminance = (BLUE_WEIGHT * pixel[0] +
                GREEN_WEIGHT * pixel[1] +
                RED_WEIGHT * pixel[2]) / WEIGHT_SUM;

            if (luminance <= m_threshold) {
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
            }
            else {
                pixel[0] = 255;
                pixel[1] = 255;
                pixel[2] = 255;
            }
        }
    }
}

std::string ThresholdFilter::name() const {
    return "threshold";
}

} // namespace ip
