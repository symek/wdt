#pragma once

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <folly/Conv.h>
#include <folly/Range.h>

namespace folly {

inline void split(char delimiter, const std::string& input,
                  std::vector<std::string>& output, bool ignoreEmpty) {
  std::string item;
  std::stringstream stream(input);
  while (std::getline(stream, item, delimiter)) {
    if (!ignoreEmpty || !item.empty()) {
      output.push_back(item);
    }
  }
  if (!ignoreEmpty && !input.empty() && input.back() == delimiter) {
    output.emplace_back();
  }
}

inline std::string humanify(const std::string& value) {
  std::ostringstream out;
  out << std::hex << std::setfill('0');
  for (unsigned char c : value) {
    if (std::isprint(c)) {
      out << static_cast<char>(c);
    } else {
      out << "\\x" << std::setw(2) << static_cast<int>(c);
    }
  }
  return out.str();
}

}  // namespace folly
