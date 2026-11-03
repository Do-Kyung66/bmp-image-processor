/**
 * @file GrayscaleFilter.cpp
 */

#include "GrayscaleFilter.h"

namespace ip {

namespace {

// RGB 가중치 공식
// 0.299 × R + 0.587 × G + 0.114 × B
constexpr int BLUE_WEIGHT = 114;
constexpr int GREEN_WEIGHT = 587;
constexpr int RED_WEIGHT = 299;
constexpr int WEIGHT_SUM = 1000;

} // anonymous namespace

void GrayscaleFilter::apply(ImageBuffer& image) const {
    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < image.width(); ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            // 픽셀 순서는 B, G, R
            const int gray = (BLUE_WEIGHT * pixel[0] +
                GREEN_WEIGHT * pixel[1] +
                RED_WEIGHT * pixel[2]) / WEIGHT_SUM;

            const std::uint8_t grayValue = static_cast<std::uint8_t>(gray);
            pixel[0] = grayValue;
            pixel[1] = grayValue;
            pixel[2] = grayValue;
        }
    }
}

std::string GrayscaleFilter::name() const {
    return "grayscale";
}

} // namespace ip
