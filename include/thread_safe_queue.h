#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

// A multi-producer / multi-consumer blocking queue.
//
// Consumers call wait_and_pop(), which sleeps until an item is available or the
// queue is stopped. The condition variable lives inside the queue (rather than
// being owned externally and paired with a separate empty() poll) so that the
// wait predicate and the pop happen under the same lock — this is what makes it
// safe for more than one consumer.
template <typename T>
class ThreadSafeQueue
{
    mutable std::mutex _mtx;
    std::condition_variable _cv;
    std::queue<T> _queue;
    bool _stopped = false;

public:
    void enqueue(const T &data)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _queue.push(data);
        }
        _cv.notify_one();
    }

    // Blocks until an item is available or the queue is stopped. On success sets
    // `out` and returns true; returns false once the queue is stopped and empty.
    bool wait_and_pop(T &out)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this] { return !_queue.empty() || _stopped; });

        if (_queue.empty()) // stopped and drained
            return false;

        out = _queue.front();
        _queue.pop();
        return true;
    }

    // Signals all waiting consumers to wake and exit once the queue drains.
    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _stopped = true;
        }
        _cv.notify_all();
    }

    [[nodiscard]] size_t size() const
    {
        std::lock_guard<std::mutex> lock(_mtx);
        return _queue.size();
    }

    [[nodiscard]] bool empty() const
    {
        std::lock_guard<std::mutex> lock(_mtx);
        return _queue.empty();
    }
};

#endif // QUEUE_H
