#pragma once

#include <optional>

namespace folly {

template <typename T>
using Optional = std::optional<T>;

inline constexpr std::nullopt_t none = std::nullopt;

}  // namespace folly
