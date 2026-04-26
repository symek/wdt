#pragma once

#include <memory>

namespace folly {

template <typename T, void (*Fn)(T*)>
struct static_function_deleter {
  void operator()(T* ptr) const {
    if (ptr != nullptr) {
      Fn(ptr);
    }
  }
};

}  // namespace folly
