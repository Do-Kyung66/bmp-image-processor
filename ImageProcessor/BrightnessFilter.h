#pragma once

/**
 * @file BrightnessFilter.h
 * @brief 이미지의 밝기를 조절하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 모든 픽셀의 B, G, R 값에 같은 값을 더한다.
 */
class BrightnessFilter : public FilterBase {
public:
    /**
     * @param brightness 더할 밝기 값 (-255~255).
     * @throws FilterError 값이 범위를 벗어난 경우.
     */
    explicit BrightnessFilter(int brightness);

    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    int m_brightness;
};
} // namespace ip
