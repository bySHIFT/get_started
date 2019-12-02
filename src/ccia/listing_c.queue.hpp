#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace messaging
{
struct message
{
  virtual ~message() {}
};

using message_shared = std::shared_ptr<message>;

template<typename T>
struct wrapped_message : public message {
    T contents;
  explicit wrapped_message(const T& contents_) : contents(contents_) {}
};

class queue
{
public:
  template<typename T>
  void push(const T& msg) {
    std::lock_guard<std::mutex> lk(m);
    q.push(std::make_shared<wrapped_message<T>>(msg));
    c.notify_all();
  }

  message_shared wait_and_pop() {
    std::unique_lock<std::mutex> lk(m);
    c.wait(lk, [&] { return !q.empty(); });
    auto res = q.front();
    q.pop();

    return res;
  }

private:
  std::mutex m;
  std::condition_variable c;
  std::queue<message_shared> q;
};

class close_queue{};

} // end namespace messaging
