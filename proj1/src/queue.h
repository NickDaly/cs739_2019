// #ifndef QUEUE_H
// #define QUEUE_H

// #include <deque>
// #include <thread>
// #include <mutex>
// #include <memory>
// #include <condition_variable>

// template <class T> 
// class queue {

// public:
//   queue();
  
//   void enqueue(T p);

//   bool dequeue_wait(T &p);

//   int size();

//   bool empty();

//   void wake_all();

//   template<typename F>
//   void apply(F func) {
//     std::for_each(queue_.begin(), queue_.end(), func);
//   }

//   void clear();

// private:
//   std::deque<T> queue_;
//   std::condition_variable monitor_;
//   std::mutex mutex_;
//   bool skip_dequeue_ = false;
// };

// #endif

#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

template <class T> 
class queue {
public:
  queue();
  void enqueue(T p);
  bool dequeue_wait(T &p);
  int size();
  bool empty();
  void wake_all();
  void clear();
private:
  std::deque<T> queue_;
  std::condition_variable monitor_;
  std::mutex mutex_;
  bool skip_dequeue_ = false;
};

#endif