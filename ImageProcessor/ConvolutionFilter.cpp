/**
 * @file ConvolutionFilter.cpp
 */

#include "ConvolutionFilter.h"
#include "Exceptions.h"

#include <algorithm>
#include <thread>
#include <vector>

namespace ip {

namespace {

constexpr int KERNEL_RADIUS = 1;

constexpr int BLUR_KERNEL[3][3] = {
    { 1, 1, 1 },
    { 1, 1, 1 },
    { 1, 1, 1 }
};
constexpr int BLUR_DIVISOR = 9;

constexpr int SHARPEN_KERNEL[3][3] = {
    {  0, -1,  0 },
    { -1,  5, -1 },
    {  0, -1,  0 }
};
constexpr int SHARPEN_DIVISOR = 1;

} // anonymous namespace

ConvolutionFilter::ConvolutionFilter(KernelType kernelType, int threadCount) : m_kernelType(kernelType), m_threadCount(threadCount) {
    if (threadCount < 1) {
        throw FilterError("Thread count must be 1 or more: " + std::to_string(threadCount));
    }
}

void ConvolutionFilter::apply(ImageBuffer& image) const {
    // 계산이 끝난 값이 다음 픽셀 계산에 섞이지 않도록 원본을 따로 둔다.
    const ImageBuffer source = image;

    // 줄 수보다 쓰레드가 많으면 빈 일을 맡는 쓰레드가 생기므로 줄 수로 제한한다.
    const int threadCount = std::min(m_threadCount, image.height());
    if (threadCount <= 1) {
        applyRows(source, image, 0, image.height());
        return;
    }

    // 이미지를 가로 띠로 나눠 쓰레드마다 맡긴다.
    // 모두 source를 읽기만 하고 서로 다른 줄에 쓰므로 잠금이 필요 없다.
    const int rowsPerThread = image.height() / threadCount;
    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    try {
        for (int i = 0; i < threadCount; ++i) {
            const int startY = i * rowsPerThread;
            // 나누어떨어지지 않고 남는 줄은 마지막 쓰레드가 맡는다.
            const int endY = (i == threadCount - 1) ? image.height() : startY + rowsPerThread;

            threads.emplace_back([this, &source, &image, startY, endY]() {
                applyRows(source, image, startY, endY);
                });
        }
    }
    catch (...) {
        // 쓰레드 생성에 실패해도 이미 시작한 쓰레드는 끝까지 기다린 뒤 예외를 전달한다.
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }

    for (std::thread& thread : threads) {
        thread.join();
    }          
}

std::string ConvolutionFilter::name() const {
    return m_kernelType == KernelType::BLUR ? "blur" : "sharpen";
}

void ConvolutionFilter::applyRows(const ImageBuffer& source, ImageBuffer& image, int startY, int endY) const {
    const bool isBlur = (m_kernelType == KernelType::BLUR);
    const auto& kernel = isBlur ? BLUR_KERNEL : SHARPEN_KERNEL;
    const int divisor = isBlur ? BLUR_DIVISOR : SHARPEN_DIVISOR;

    for (int y = startY; y < endY; ++y) {
        std::uint8_t* row = image.rowPtr(y);

        for (int x = 0; x < image.width(); ++x) {
            for (int channel = 0; channel < ImageBuffer::CHANNELS; ++channel) {
                int sum = 0;

                for (int kernelY = -KERNEL_RADIUS; kernelY <= KERNEL_RADIUS; ++kernelY) {
                    for (int kernelX = -KERNEL_RADIUS; kernelX <= KERNEL_RADIUS; ++kernelX) {
                        // 이미지 밖의 좌표는 가장 가까운 가장자리로 고정한다.
                        const int sampleX = std::clamp(x + kernelX, 0, image.width() - 1);
                        const int sampleY = std::clamp(y + kernelY, 0, image.height() - 1);

                        // 원본 이미지에서 해당 좌표의 값을 꺼낸다.
                        const int value = source.rowPtr(sampleY)[sampleX * ImageBuffer::CHANNELS + channel];

                        // 칸마다 커널 값을 곱해 합계에 더한다.
                        sum += value * kernel[kernelY + KERNEL_RADIUS][kernelX + KERNEL_RADIUS];
                    }
                }
                // 커널 값의 합으로 나눠 평균을 구하고, 0~255로 제한해 저장한다.
                const int result = std::clamp(sum / divisor, 0, 255);
                row[x * ImageBuffer::CHANNELS + channel] = static_cast<std::uint8_t>(result);
            }
        }
    }
}

} // namespace ip


