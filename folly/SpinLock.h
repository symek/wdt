#pragma once

#include <mutex>

namespace folly {

class SpinLock {
 public:
  void lock() { mutex_.lock(); }
  void unlock() { mutex_.unlock(); }
  bool try_lock() { return mutex_.try_lock(); }

 private:
  std::mutex mutex_;
};

}  // namespace folly
