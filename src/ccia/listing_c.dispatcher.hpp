#pragma once
#include "listing_c.queue.hpp"
#include "listing_c.template_dispatcher.hpp"

namespace messaging
{
class dispatcher
{
public:
  explicit dispatcher(queue* q_) : q(q_), chained(false) {}
  dispatcher(dispatcher&& other)
    : q(other.q)
    , chained(other.chained)
  { other.chained = true; }

  ~dispatcher() noexcept(false) { if (!chained) wait_and_dispatch(); }

  dispatcher(const dispatcher&) = delete;
  dispatcher& operator=(const dispatcher&) = delete;

  template<typename Message, typename Func>
  auto // TemplateDispatcher<dispatcher, Message, Func>
  handle(Func&& f) {
    return TemplateDispatcher<dispatcher, Message, Func>\
      { this, q, std::forward<Func>(f) };
  }

private:
  template<typename Dispatcher, typename Message, typename Func>
  friend class TemplateDispatcher;

  void wait_and_dispatch() {
    while (true) { dispatch(q->wait_and_pop()); }
  }

  bool dispatch(const message_shared& msg) {
    if (std::dynamic_pointer_cast<wrapped_message<close_queue>>(msg))\
      throw close_queue();

    return false;
  }

  queue* q { nullptr };
  bool   chained { false };
};
} // end namespace messaging
