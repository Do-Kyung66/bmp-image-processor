/**
 * @file ResizeFilter.cpp
 */

#include "ResizeFilter.h"
#include "Exceptions.h"

#include <algorithm>   // std::copy_n
#include <utility>     // std::move

namespace ip {

namespace {

constexpr int MAX_SIZE = 10000;

} // anonymous namespace

ResizeFilter::ResizeFilter(int width, int height) : m_width(width), m_height(height) {
    if (width < 1 || width > MAX_SIZE || height < 1 || height > MAX_SIZE) {
        throw FilterError("Resize size must be 1-" + std::to_string(MAX_SIZE) + ": "
            + std::to_string(width) + ":" + std::to_string(height));
    }
}

void ResizeFilter::apply(ImageBuffer& image) const {
    ImageBuffer result(m_width, m_height);

    for (int y = 0; y < m_height; ++y) {
        const int sourceY = static_cast<int>(static_cast<long long>(y) * image.height() / m_height);

        const std::uint8_t* sourceRow = image.rowPtr(sourceY);
        std::uint8_t* resultRow = result.rowPtr(y);

        for (int x = 0; x < m_width; ++x) {
            const int sourceX = static_cast<int>(static_cast<long long>(x) * image.width() / m_width);

            std::copy_n(sourceRow + sourceX * ImageBuffer::CHANNELS,
                ImageBuffer::CHANNELS,
                resultRow + x * ImageBuffer::CHANNELS);
        }
    }

    image = std::move(result);
}

std::string ResizeFilter::name() const {
    return "resize";
}

} // namespace ip