#pragma once

#include <sstream>
#include <string>
#include <type_traits>

namespace folly {

template <typename T, typename U>
T to(const U& value) {
  if constexpr (std::is_same_v<T, std::string>) {
    if constexpr (std::is_same_v<std::decay_t<U>, bool>) {
      return value ? "true" : "false";
    } else {
      std::ostringstream out;
      out << value;
      return out.str();
    }
  } else if constexpr (std::is_same_v<T, bool>) {
    if constexpr (std::is_same_v<std::decay_t<U>, std::string>) {
      return value == "1" || value == "true" || value == "True";
    } else {
      return static_cast<bool>(value);
    }
  } else {
    std::istringstream in(value);
    T result{};
    in >> result;
    return result;
  }
}

template <>
inline std::string to<std::string, std::string>(const std::string& value) {
  return value;
}

template <typename T>
void appendOne(std::string* out, const T& value) {
  out->append(to<std::string>(value));
}

inline void appendOne(std::string* out, const char* value) {
  out->append(value);
}

inline void appendOne(std::string* out, char value) {
  out->push_back(value);
}

inline void appendOne(std::string* out, const std::string& value) {
  out->append(value);
}

template <typename A>
void toAppend(const A& a, std::string* out) {
  appendOne(out, a);
}

template <typename A, typename B>
void toAppend(const A& a, const B& b, std::string* out) {
  appendOne(out, a);
  appendOne(out, b);
}

template <typename A, typename B, typename C>
void toAppend(const A& a, const B& b, const C& c, std::string* out) {
  appendOne(out, a);
  appendOne(out, b);
  appendOne(out, c);
}

template <typename A, typename B, typename C, typename D>
void toAppend(const A& a, const B& b, const C& c, const D& d,
              std::string* out) {
  appendOne(out, a);
  appendOne(out, b);
  appendOne(out, c);
  appendOne(out, d);
}

}  // namespace folly
