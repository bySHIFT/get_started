#pragma once
#include "listing_c.queue.hpp"

namespace messaging
{
template<typename PreviousDispatcher, typename Msg, typename Func>
class TemplateDispatcher
{
private:
  template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
  friend class TemplateDispatcher;

public:
  TemplateDispatcher(TemplateDispatcher&& other)
    : q(other.q)
    , prev(other.prev)
    , f(std::move(other.f))
    , chained(other.chained)
  { other.chained = true; }

  TemplateDispatcher(queue* q_, PreviousDispatcher* prev_, Func&& f_)
    : q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false)
  { prev_->chained = true; }

  template<typename OtherMsg, typename OtherFunc>
  TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
  handle(OtherFunc&& of) {
    return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> \
      (q, this, std::forward<OtherFunc>(of));
  }

  ~TemplateDispatcher() noexcept(false) {
    if(!chained) wait_and_dispatch();
  }

private:
  queue* q { nullptr };
  PreviousDispatcher* prev { nullptr };
  Func f;
  bool chained { false };

  TemplateDispatcher(const TemplateDispatcher&) = delete;
  TemplateDispatcher& operator=(const TemplateDispatcher&) = delete;

  void wait_and_dispatch() {
    while (true) {
      auto msg = q->wait_and_pop();
      if (dispatch(msg)) break;
    }
  }

  bool dispatch(const std::shared_ptr<message_base>& msg) {
    if (wrapped_message<Msg>* wrapper = dynamic_cast<wrapped_message<Msg>*>(msg.get())) {
      f(wrapper->contents);
      return true;
    } else {
      return prev->dispatch(msg);
    }
  }
};
} // end namespace messaging
