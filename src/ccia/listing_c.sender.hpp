#pragma once
#include "listing_c.queue.hpp"

namespace messaging
{
class sender
{
public:
  sender() : q(nullptr) {}
  explicit sender(queue* q_) : q(q_) {}

  template<typename Message>
  void send(const Message& msg)
  {
    if (q) q->push(msg);
  }

private:
  queue* q { nullptr };
};
} // end namespace messaging
