#pragma once

#include <utility>

namespace folly {

template <typename F>
class ScopeGuard {
 public:
  explicit ScopeGuard(F&& f) : f_(std::forward<F>(f)) {}
  ScopeGuard(ScopeGuard&& other) noexcept
      : f_(std::move(other.f_)), active_(other.active_) {
    other.dismiss();
  }
  ~ScopeGuard() {
    if (active_) {
      f_();
    }
  }
  void dismiss() { active_ = false; }

 private:
  F f_;
  bool active_{true};
};

template <typename F>
ScopeGuard<F> makeGuard(F&& f) {
  return ScopeGuard<F>(std::forward<F>(f));
}

}  // namespace folly
