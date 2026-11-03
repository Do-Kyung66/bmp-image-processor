/**
 * @file BrightnessFilter.cpp
 */

#include "BrightnessFilter.h"
#include "Exceptions.h"

#include <algorithm>

namespace ip {

BrightnessFilter::BrightnessFilter(int brightness) : m_brightness(brightness) {
    if (brightness < -255 || brightness > 255) {
        throw FilterError("Brightness must be -255 to 255: " + std::to_string(brightness));
    }
}

void BrightnessFilter::apply(ImageBuffer& image) const {
    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < image.width(); ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            for (int channel = 0; channel < ImageBuffer::CHANNELS; ++channel) {
                const int adjustedValue = pixel[channel] + m_brightness;
                const int clampedValue = std::clamp(adjustedValue, 0, 255);

                pixel[channel] = static_cast<std::uint8_t>(clampedValue);
            }
        }
    }
}

std::string BrightnessFilter::name() const {
    return "brightness";
}

} // namespace ip
