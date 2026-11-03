#pragma once

/**
 * @file FilterBase.h
 * @brief 모든 이미지 필터의 추상 기반 클래스.
 */

#include "ImageBuffer.h"

#include <string>

namespace ip {

/**
 * @brief 이미지 필터의 공통 인터페이스.
 * 필터는 이 클래스를 상속받는다.
 */
class FilterBase {
public:
    virtual ~FilterBase() = default;

    /**
     * @brief 이미지에 필터를 적용한다.
     * @throws FilterError 필터 적용에 실패한 경우.
     */
    virtual void apply(ImageBuffer& image) const = 0;

    /// 로그 출력에 사용할 필터 이름을 반환한다.
    virtual std::string name() const = 0;
};

} // namespace ip
