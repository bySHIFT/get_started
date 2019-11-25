#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace messaging
{
struct message_base
{
  virtual ~message_base() {}
};

template<typename Msg>
struct wrapped_message : message_base
{
  Msg contents;
  explicit wrapped_message(const Msg& contents_) : contents(contents_) {}
};

class queue
{
public:
  template<typename T>
  void push(const T& msg)
  {
    std::lock_guard<std::mutex> lk(m);
    q.push(std::make_shared<wrapped_message<T>>(msg));
    c.notify_all();
  }

  std::shared_ptr<message_base> wait_and_pop()
  {
    std::unique_lock<std::mutex> lk(m);
    c.wait(lk, [&] { return !q.empty(); });
    auto res=q.front();
    q.pop();

    return res;
  }

private:
  std::mutex m;
  std::condition_variable c;
  std::queue<std::shared_ptr<message_base>> q;
};
} // end namespace messaging
