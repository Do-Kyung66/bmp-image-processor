/**
 * @file ContrastFilter.cpp
 */

#include "ContrastFilter.h"
#include "Exceptions.h"

#include <algorithm>

namespace ip {

namespace {

constexpr double MID_VALUE = 128.0;

} // anonymous namespace

ContrastFilter::ContrastFilter(double contrast) : m_contrast(contrast) {
    if (!(contrast >= 0.0 && contrast <= 10.0)) {
        throw FilterError("Contrast must be 0.0 to 10.0: " + std::to_string(contrast));
    }
}

void ContrastFilter::apply(ImageBuffer& image) const {
    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < image.width(); ++x) {
            std::uint8_t* pixel = row + x * ImageBuffer::CHANNELS;

            for (int channel = 0; channel < ImageBuffer::CHANNELS; ++channel) {
                const double adjustedValue = (pixel[channel] - MID_VALUE) * m_contrast + MID_VALUE;
                const double clampedValue = std::clamp(adjustedValue, 0.0, 255.0);

                pixel[channel] = static_cast<std::uint8_t>(clampedValue);
            }
        }
    }
}

std::string ContrastFilter::name() const {
    return "contrast";
}

} // namespace ip
