#pragma once
#include "listing_c.sender.hpp"
#include "listing_c.dispatcher.hpp"

namespace messaging
{
class receiver
{
public:
  explicit operator sender() { return sender(&q); }

  dispatcher wait() { return dispatcher(&q); }

private:
  queue q {};
};
} // end namespace messaging
