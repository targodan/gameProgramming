#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

namespace engine {
    namespace util {
        template<typename T>
        class BlockingQueue
        {
        private:
            std::mutex              d_mutex;
            std::condition_variable d_condition;
            std::deque<T>           d_queue;
        public:
            BlockingQueue() {}
            ~BlockingQueue() {
                d_queue.clear();
            }
            
            void push(T& value) {
                {
                    std::unique_lock<std::mutex> lock(this->d_mutex);
                    d_queue.push_front(value);
                }
                this->d_condition.notify_one();
            }
            void push(T&& value) {
                {
                    std::unique_lock<std::mutex> lock(this->d_mutex);
                    d_queue.push_front(std::move(value));
                }
                this->d_condition.notify_one();
            }
            T pop() {
                std::unique_lock<std::mutex> lock(this->d_mutex);
                this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
                T rc(std::move(this->d_queue.back()));
                this->d_queue.pop_back();
                return rc;
            }
            void resize(size_t newSize) {
                this->d_queue.resize(newSize);
            }
            void clear() {
                this->d_queue.clear();
            }
        };
    }
}

#endif /* BLOCKING_QUEUE_H */