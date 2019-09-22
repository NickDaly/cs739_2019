#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

template <class T> 
class queue {

private:
  std::deque<T> queue_;
  std::condition_variable monitor_;
  std::mutex mutex_;
  bool skip_dequeue_ = false;

public:

  queue() {
  
  }

  void enqueue(T x) {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      queue_.push_back(x);
    }
    monitor_.notify_one();
  }

  bool dequeue_wait(T &x) {
    std::unique_lock<std::mutex> lock(mutex_);

    skip_dequeue_ = false;

    monitor_.wait(lock, [this] {
     return skip_dequeue_ || !queue_.empty();
    });

    if (skip_dequeue_) return false;

    if (queue_.empty()) return false;

    x = queue_.front();
    queue_.pop_front();

    return true;
  }

  int size() {
    return queue_.size();
  }

  bool empty() {
    return queue_.empty();
  }

  void wake_all() {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      skip_dequeue_ = true;
    }
      monitor_.notify_all();
  }

  void clear() {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      skip_dequeue_ = true;
      queue_.clear();
    }
    monitor_.notify_all();
  }

};

#endif