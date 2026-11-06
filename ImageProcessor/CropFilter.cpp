/**
 * @file CropFilter.cpp
 */

#include "CropFilter.h"
#include "Exceptions.h"

#include <algorithm>   // std::copy_n
#include <utility>     // std::move

namespace ip {

CropFilter::CropFilter(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height) {
    if (x < 0 || y < 0 || width <= 0 || height <= 0) {
        throw FilterError("Crop x and y must be 0 or more, width and height must be positive: "
            + std::to_string(x) + ":" + std::to_string(y) + ":"
            + std::to_string(width) + ":" + std::to_string(height));
    }
}

void CropFilter::apply(ImageBuffer& image) const {
    if (m_x >= image.width() || m_y >= image.height() ||
        m_width > image.width() - m_x || m_height > image.height() - m_y) {
        throw FilterError("Crop area is outside the image");
    }

    ImageBuffer result(m_width, m_height);

    for (int row = 0; row < m_height; ++row) {
        const std::uint8_t* sourceRow = image.rowPtr(m_y + row) + m_x * ImageBuffer::CHANNELS;
        std::copy_n(sourceRow, result.rowStride(), result.rowPtr(row));
    }

    image = std::move(result);
}

std::string CropFilter::name() const {
    return "crop";
}

} // namespace ip

