#pragma once

/**
 * @file ResizeFilter.h
 * @brief 이미지를 지정한 크기로 확대하거나 축소하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 결과의 각 픽셀에 원본에서 가장 가까운 픽셀을 복사한다.(최근접 이웃 보간법)
 */
class ResizeFilter : public FilterBase {
public:
    /**
     * @param width  결과 이미지의 너비 (1~10000).
     * @param height 결과 이미지의 높이 (1~10000).
     * @throws FilterError 너비나 높이가 범위를 벗어난 경우.
     */
    ResizeFilter(int width, int height);
    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    int m_width;
    int m_height;
};
} // namespace ip
