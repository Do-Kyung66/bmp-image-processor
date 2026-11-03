#pragma once

/**
 * @file GrayscaleFilter.h
 * @brief 컬러 이미지를 흑백으로 변환하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 각 픽셀을 RGB 가중치로 계산한 밝기값의 회색으로 바꾼다.
 */
class GrayscaleFilter : public FilterBase {
public:
    void apply(ImageBuffer& image) const override;
    std::string name() const override;
};

} // namespace ip
