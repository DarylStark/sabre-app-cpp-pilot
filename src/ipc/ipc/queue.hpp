#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

namespace ipc
{
    template <typename MessageType>
    class Queue
    {
    public:
        using Ptr = Queue<MessageType> *;
        using SharedPtr = std::shared_ptr<Queue<MessageType>>;
        using UniquePtr = std::unique_ptr<Queue<MessageType>>;

    private:
        std::mutex _mutex;
        std::queue<MessageType> _queue;
        std::condition_variable _cv;
        bool _isStopping = false;

    public:
        void push(const MessageType &message)
        {
            bool changed = false;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (!_isStopping)
                {
                    _queue.push(message);
                    changed = true;
                }
            }
            if (changed)
            {
                _cv.notify_one();
            }
        }

        void push(MessageType &&message)
        {
            bool changed = false;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (!_isStopping)
                {
                    _queue.push(std::move(message));
                    changed = true;
                }
            }
            if (changed)
            {
                _cv.notify_one();
            }
        }

        template <typename... Args>
        void emplace(Args &&...args)
        {
            bool changed = false;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (!_isStopping)
                {
                    _queue.emplace(std::forward<Args>(args)...);
                    changed = true;
                }
            }
            if (changed)
            {
                _cv.notify_one();
            }
        }

        std::optional<MessageType> pop()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this] { return !_queue.empty() || _isStopping; });

            if (_isStopping)
                return std::nullopt;

            MessageType item = std::move(_queue.front());
            _queue.pop();

            return item;
        }

        std::optional<MessageType> try_pop()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty())
            {
                return std::nullopt;
            }

            MessageType item = std::move(_queue.front());
            _queue.pop();

            return item;
        }

        void shutdown()
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _isStopping = true;
                while (!_queue.empty())
                {
                    _queue.pop();
                }
            }
            _cv.notify_all();
        }
    };
} // namespace ipc