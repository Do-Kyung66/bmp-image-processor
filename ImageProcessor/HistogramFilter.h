#pragma once

/**
 * @file HistogramFilter.h
 * @brief 밝기 분포를 콘솔에 출력하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 밝기별로 픽셀이 몇 개인지 세어 콘솔에 막대로 보여준다.
 */
class HistogramFilter : public FilterBase {
public:
    void apply(ImageBuffer& image) const override;
    std::string name() const override;
};

} // namespace ip