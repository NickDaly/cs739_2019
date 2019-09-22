#include "queue.h"


template <class T> 
queue<T>::queue() {

}


template <class T> 
void queue<T>::enqueue(T x) {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push_back(x);
    }
    monitor_.notify_one();
}


template <class T> 
bool queue<T>::dequeue_wait(T &x) {
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


template <class T> 
int queue<T>::size() {
	return queue_.size();
}


template <class T> 
bool queue<T>::empty() {
	return queue_.empty();
}


template <class T> 
void queue<T>::wake_all() {
	{
		std::unique_lock<std::mutex> lock(mutex_);
		skip_dequeue_ = true;
	}
	monitor_.notify_all();
}


template <class T> 
void queue<T>::clear() {
	{
		std::unique_lock<std::mutex> lock(mutex_);
		skip_dequeue_ = true;
		queue_.clear();
	}
	monitor_.notify_all();
}