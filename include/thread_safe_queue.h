#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>

template <typename T>
class ThreadSafeQueue
{
    std::mutex _mtx;
    std::queue<T> _queue;

public:
    void enqueue(const T &data)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _queue.push(data);
    }

    T dequeue()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        const T data = _queue.front();
        _queue.pop();

        return data;
    }

    [[nodiscard]] size_t size() const
    {
        return _queue.size();
    }

    [[nodiscard]] bool empty() const
    {
        return _queue.empty();
    }
};

#endif // QUEUE_H