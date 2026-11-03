#pragma once

/**
 * @file ThresholdFilter.h
 * @brief 이미지를 흑백으로 구분하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 밝기가 임계값 이하인 픽셀은 검은색, 초과인 픽셀은 흰색으로 바꾼다.
 */
class ThresholdFilter : public FilterBase {
public:
    /**
     * @param threshold 임계값 (0~255).
     * @throws FilterError 임계값이 범위를 벗어난 경우.
     */
    explicit ThresholdFilter(int threshold);

    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    int m_threshold;
};

} // namespace ip
