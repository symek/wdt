#pragma once

#include <shared_mutex>

namespace folly {

class RWSpinLock {
 public:
  void lock() { mutex_.lock(); }
  void unlock() { mutex_.unlock(); }
  bool try_lock() { return mutex_.try_lock(); }
  void lock_shared() { mutex_.lock_shared(); }
  void unlock_shared() { mutex_.unlock_shared(); }
  bool try_lock_shared() { return mutex_.try_lock_shared(); }

 private:
  std::shared_mutex mutex_;
};

}  // namespace folly
