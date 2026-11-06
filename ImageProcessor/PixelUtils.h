#pragma once

/**
 * @file PixelUtils.h
 * @brief 여러 필터가 함께 쓰는 픽셀 계산 함수.
 */

#include <cstdint>

namespace ip {

/**
 * @brief BGR 픽셀의 밝기를 BT.601 가중치로 계산한다.
 * @param pixel B, G, R 순서로 저장된 픽셀의 시작 포인터.
 * @return 밝기 (0~255).
 */
inline int calculateLuminance(const std::uint8_t* pixel) {
    constexpr int BLUE_WEIGHT = 114;
    constexpr int GREEN_WEIGHT = 587;
    constexpr int RED_WEIGHT = 299;
    constexpr int WEIGHT_SUM = 1000;

    return (BLUE_WEIGHT * pixel[0] + GREEN_WEIGHT * pixel[1] + RED_WEIGHT * pixel[2]) / WEIGHT_SUM;
}

} // namespace ip