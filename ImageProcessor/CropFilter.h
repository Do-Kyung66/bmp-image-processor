#pragma once

/**
 * @file CropFilter.h
 * @brief 이미지에서 지정한 사각형 영역만 잘라내는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 시작 좌표와 크기로 정한 영역만 남겨 이미지 크기를 바꾼다.
 */
class CropFilter : public FilterBase {
public:
    /**
     * @param x      잘라낼 영역의 왼쪽 위 x 좌표 (0 이상).
     * @param y      잘라낼 영역의 왼쪽 위 y 좌표 (0 이상).
     * @param width  잘라낼 영역의 너비 (양수).
     * @param height 잘라낼 영역의 높이 (양수).
     * @throws FilterError 좌표가 음수이거나 크기가 0 이하인 경우.
     */
    CropFilter(int x, int y, int width, int height);
    /**
     * @throws FilterError 잘라낼 영역이 이미지 범위를 벗어난 경우.
     */
    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};
} // namespace ip
