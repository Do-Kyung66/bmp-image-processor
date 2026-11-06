/**
 * @file GrayscaleFilter.cpp
 */

#include "GrayscaleFilter.h"
#include "PixelUtils.h"

namespace ip {

void GrayscaleFilter::apply(ImageBuffer& image) const {
    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < image.width(); ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            // 픽셀 순서는 B, G, R
            const int gray = calculateLuminance(pixel);

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
