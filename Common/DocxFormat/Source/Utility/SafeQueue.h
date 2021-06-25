#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
    , isEmpty(true)
  {}

  ~SafeQueue(void)
  {}

  // Add an element to the queue.
  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    isEmpty = false;
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      if(isEmpty)
        return nullptr;
      c.wait(lock);
      if(isEmpty)
        return nullptr;
    }
    T val = q.front();
    q.pop();
    return val;
  }

  void stopwait()
  {
      isEmpty = true;
      c.notify_one();
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
  bool isEmpty;
};
#endif // SAFEQUEUE_H
