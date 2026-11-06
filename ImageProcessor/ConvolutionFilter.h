#pragma once

/**
 * @file ConvolutionFilter.h
 * @brief 3×3 합성곱으로 이미지를 흐리게 하거나 선명하게 하는 필터.
 */

#include "FilterBase.h"

namespace ip {
/**
 * @brief 각 픽셀을 주변 9칸과 커널로 계산한 값으로 바꾼다.
 */
class ConvolutionFilter : public FilterBase {
public:
    /// 적용할 커널 종류.
    enum class KernelType {
        BLUR,   ///< 주변 9칸의 평균으로 흐리게 한다.
        SHARPEN ///< 주변과의 차이를 키워 선명하게 한다.
    };

    /**
     * @param kernelType 적용할 커널 종류.
     * @param threadCount 계산을 나눠 맡을 쓰레드 수. 1이면 나누지 않는다.
     * @throws FilterError threadCount가 1보다 작은 경우.
     */
    explicit ConvolutionFilter(KernelType kernelType, int threadCount = 1);

    void apply(ImageBuffer& image) const override;
    std::string name() const override;

private:
    /// source를 읽어 image의 startY부터 endY 전까지의 줄을 계산한다.
    void applyRows(const ImageBuffer& source, ImageBuffer& image, int startY, int endY) const;

    KernelType m_kernelType;
    int m_threadCount;
};
} // namespace ip
