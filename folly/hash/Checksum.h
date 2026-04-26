#pragma once

#include <cstddef>
#include <cstdint>

namespace folly {

inline uint32_t crc32c(const uint8_t* data, std::size_t size,
                       uint32_t crc = 0) {
  crc = ~crc;
  for (std::size_t i = 0; i < size; ++i) {
    crc ^= data[i];
    for (int bit = 0; bit < 8; ++bit) {
      crc = (crc >> 1) ^ (0x82f63b78u & static_cast<uint32_t>(-(crc & 1)));
    }
  }
  return ~crc;
}

}  // namespace folly
