#pragma once

/**
 * @file FilterFactory.h
 * @brief 필터 이름으로 필터 객체를 만드는 함수.
 */

#include "FilterBase.h"

#include <memory>
#include <string>
#include <vector>

namespace ip {

/**
 * @brief 이름에 맞는 필터를 만든다.
 * @param filterName 필터 이름.
 * @param threadCount 멀티쓰레드 갯수.
 * @return 만들어진 필터.
 * @throws FilterError 이름을 알 수 없거나 값이 잘못된 경우.
 */
std::unique_ptr<FilterBase> createFilter(const std::string& filterName, int threadCount);
/**
 * @brief 쉼표로 구분한 목록에서 필터를 순서대로 만든다.
 * @param pipeline 필터 목록.
 * @param threadCount 멀티쓰레드 갯수.
 * @return 만들어진 필터 목록.
 * @throws FilterError 이름이 비어 있거나 잘못된 경우.
 */
std::vector<std::unique_ptr<FilterBase>> createPipeline(const std::string& pipeline, int threadCount);
} // namespace ip