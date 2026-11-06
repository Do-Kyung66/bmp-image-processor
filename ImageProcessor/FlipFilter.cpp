/**
 * @file FlipFilter.cpp
 */

#include "FlipFilter.h"

#include <utility>

namespace ip {

FlipFilter::FlipFilter(Direction direction) : m_direction(direction) {
}

void FlipFilter::apply(ImageBuffer& image) const {
    if (m_direction == Direction::HORIZONTAL){
        flipHorizontal(image);
    }
    else {
        flipVertical(image);
    }
}

std::string FlipFilter::name() const {
    return m_direction == Direction::HORIZONTAL ? "flip:horizontal" : "flip:vertical";
}

void FlipFilter::flipHorizontal(ImageBuffer& image) const {
    for (int y = 0; y < image.height(); ++y) {
        std::uint8_t* row = image.rowPtr(y);
        for (int x = 0; x < image.width() / 2; ++x) {
            const int mirrorX = image.width() - 1 - x;

            std::uint8_t* leftPixel = row + x * ImageBuffer::CHANNELS;
            std::uint8_t* rightPixel = row + mirrorX * ImageBuffer::CHANNELS;

            for (int channel = 0; channel < ImageBuffer::CHANNELS; ++channel) {
                std::swap(leftPixel[channel], rightPixel[channel]);
            }
        }
    }
}

void FlipFilter::flipVertical(ImageBuffer& image) const {
    for (int y = 0; y < image.height() / 2; ++y) {
        std::uint8_t* topRow = image.rowPtr(y);
        std::uint8_t* bottomRow = image.rowPtr(image.height() - 1 - y);

        for (int byteIndex = 0; byteIndex < image.rowStride(); ++byteIndex) {
            std::swap(topRow[byteIndex], bottomRow[byteIndex]);
        }
    }
}

} // namespace ip

