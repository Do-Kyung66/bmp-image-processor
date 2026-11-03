#pragma once

/**
 * @file ContrastFilter.h
 * @brief 이미지의 대비를 조절하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 128을 기준으로 픽셀 값의 차이를 배율만큼 늘리거나 줄인다.
 */
class ContrastFilter : public FilterBase {
public:
    /**
     * @param contrast 대비 배율 (0.0~10.0). 1.0이면 원본과 같다.
     * @throws FilterError 값이 범위를 벗어난 경우.
     */
    explicit ContrastFilter(double contrast);

    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    double m_contrast;
};

} // namespace ip
