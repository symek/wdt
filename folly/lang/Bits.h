#pragma once

#include <cstring>
#include <type_traits>

namespace folly {

template <typename T>
T loadUnaligned(const void* ptr) {
  T value;
  std::memcpy(&value, ptr, sizeof(T));
  return value;
}

template <typename T>
void storeUnaligned(void* ptr, T value) {
  std::memcpy(ptr, &value, sizeof(T));
}

struct Endian {
  template <typename T>
  static T little(T value) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return value;
#else
    if constexpr (sizeof(T) == 1) {
      return value;
    } else if constexpr (sizeof(T) == 2) {
      using U = std::make_unsigned_t<T>;
      return static_cast<T>(__builtin_bswap16(static_cast<U>(value)));
    } else if constexpr (sizeof(T) == 4) {
      using U = std::make_unsigned_t<T>;
      return static_cast<T>(__builtin_bswap32(static_cast<U>(value)));
    } else if constexpr (sizeof(T) == 8) {
      using U = std::make_unsigned_t<T>;
      return static_cast<T>(__builtin_bswap64(static_cast<U>(value)));
    }
#endif
  }
};

}  // namespace folly
