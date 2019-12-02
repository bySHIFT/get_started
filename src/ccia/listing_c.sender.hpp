#pragma once
#include "listing_c.queue.hpp"

namespace messaging
{
class sender
{
public:
  explicit sender(queue* q_ = nullptr) : q(q_) {}

  template<typename T>
  void send(const T& msg) { if (q) q->push(msg); }

private:
  queue* q { nullptr };
};
} // end namespace messaging
