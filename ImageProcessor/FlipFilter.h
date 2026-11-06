#pragma once

/**
 * @file FlipFilter.h
 * @brief 이미지의 반전을 하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 픽셀의 위치만 맞바꾸는 필터이다.
 */
class FlipFilter : public FilterBase {
public:
    /// 반전 방향.
    enum class Direction {
        HORIZONTAL, ///< 좌우 반전.
        VERTICAL    ///< 상하 반전.
    };

    /**
     * @param direction 반전 방향.
     */
    explicit FlipFilter(Direction direction);

    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    void flipHorizontal(ImageBuffer& image) const;
    void flipVertical(ImageBuffer& image) const;

    Direction m_direction;
};
} // namespace ip
