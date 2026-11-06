/**
 * @file ThresholdFilter.cpp
 */

#include "ThresholdFilter.h"
#include "PixelUtils.h"
#include "Exceptions.h"

namespace ip {

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

            const int luminance = calculateLuminance(pixel);

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
