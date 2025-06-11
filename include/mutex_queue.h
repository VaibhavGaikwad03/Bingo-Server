#ifndef QUEUE_H
#define QUEUE_H

// #define QUEUE_BUFFER_SIZE 4096

#include <queue>
#include <mutex>

// #pragma pack(push, 2)
//
// #pragma pack(pop)

template <typename T>
class MutexQueue
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