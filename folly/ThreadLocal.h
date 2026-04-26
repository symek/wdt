#pragma once

#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace folly {

template <typename T, typename Tag = void>
class ThreadLocalPtr {
 public:
  ThreadLocalPtr() = default;
  ~ThreadLocalPtr() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& entry : data_) {
      delete entry.second;
    }
  }

  T* get() {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(std::this_thread::get_id());
    return it == data_.end() ? nullptr : it->second;
  }

  void reset(T* ptr = nullptr) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& slot = data_[std::this_thread::get_id()];
    delete slot;
    slot = ptr;
  }

  class AccessAllThreads {
   public:
    explicit AccessAllThreads(std::vector<T*> items) : items_(std::move(items)) {}

    class Iterator {
     public:
      using Inner = typename std::vector<T*>::iterator;
      explicit Iterator(Inner it) : it_(it) {}
      Iterator& operator++() {
        ++it_;
        return *this;
      }
      bool operator!=(const Iterator& other) const { return it_ != other.it_; }
      T& operator*() const { return **it_; }

     private:
      Inner it_;
    };

    Iterator begin() { return Iterator(items_.begin()); }
    Iterator end() { return Iterator(items_.end()); }

   private:
    std::vector<T*> items_;
  };

  AccessAllThreads accessAllThreads() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<T*> items;
    items.reserve(data_.size());
    for (auto& entry : data_) {
      if (entry.second != nullptr) {
        items.push_back(entry.second);
      }
    }
    return AccessAllThreads(std::move(items));
  }

 private:
  std::mutex mutex_;
  std::unordered_map<std::thread::id, T*> data_;
};

template <typename T>
class ThreadLocal {
 public:
  T* get() { return ptr_.get(); }
  void reset(T* value = nullptr) { ptr_.reset(value); }

 private:
  ThreadLocalPtr<T> ptr_;
};

}  // namespace folly
