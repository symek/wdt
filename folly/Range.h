#pragma once

#include <cstdint>
#include <cstring>
#include <ostream>
#include <string>

namespace folly {

class ByteRange {
 public:
  ByteRange() = default;
  ByteRange(uint8_t* data, std::size_t size) : begin_(data), end_(data + size) {}
  ByteRange(const uint8_t* data, std::size_t size)
      : begin_(const_cast<uint8_t*>(data)), end_(const_cast<uint8_t*>(data) + size) {}

  uint8_t* start() const { return begin_; }
  uint8_t* end() const { return end_; }
  std::size_t size() const { return static_cast<std::size_t>(end_ - begin_); }
  bool empty() const { return begin_ == end_; }
  uint8_t front() const { return *begin_; }
  void advance(std::size_t n) {
    begin_ += n;
    if (begin_ > end_) {
      begin_ = end_;
    }
  }
  void pop_front() { advance(1); }

 private:
  uint8_t* begin_{nullptr};
  uint8_t* end_{nullptr};
};

class StringPiece {
 public:
  StringPiece() = default;
  StringPiece(const std::string& str) : data_(str.data()), size_(str.size()) {}
  StringPiece(const std::string& str, std::size_t pos)
      : data_(str.data() + pos), size_(str.size() - pos) {}
  StringPiece(const std::string& str, std::size_t pos, std::size_t len)
      : data_(str.data() + pos), size_(len) {}
  StringPiece(const char* data, std::size_t size) : data_(data), size_(size) {}

  const char* data() const { return data_; }
  std::size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }
  char operator[](std::size_t i) const { return data_[i]; }
  std::size_t find(char c) const {
    for (std::size_t i = 0; i < size_; ++i) {
      if (data_[i] == c) {
        return i;
      }
    }
    return std::string::npos;
  }
  std::string toString() const { return std::string(data_, size_); }
  operator std::string() const { return std::string(data_, size_); }

  void advance(std::size_t n) {
    if (n > size_) {
      n = size_;
    }
    data_ += n;
    size_ -= n;
  }

  StringPiece split_step(char delimiter) {
    for (std::size_t i = 0; i < size_; ++i) {
      if (data_[i] == delimiter) {
        StringPiece result(data_, i);
        advance(i + 1);
        return result;
      }
    }
    StringPiece result(data_, size_);
    advance(size_);
    return result;
  }

  friend bool operator==(const StringPiece& lhs, const StringPiece& rhs) {
    return lhs.size_ == rhs.size_ &&
           std::memcmp(lhs.data_, rhs.data_, lhs.size_) == 0;
  }

  friend bool operator!=(const StringPiece& lhs, const StringPiece& rhs) {
    return !(lhs == rhs);
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const StringPiece& piece) {
    return os.write(piece.data_, piece.size_);
  }

 private:
  const char* data_{nullptr};
  std::size_t size_{0};
};

}  // namespace folly
