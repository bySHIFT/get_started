#pragma once
#include "listing_c.queue.hpp"

namespace messaging
{
template<typename PreviousDispatcher, typename Message, typename Func>
class TemplateDispatcher
{
public:
  TemplateDispatcher(PreviousDispatcher* prev_, queue* q_, Func&& f_)
    : prev(prev_), q(q_), f(std::forward<Func>(f_)), chained(false)
  { prev_->chained = true; }
  TemplateDispatcher(TemplateDispatcher&& other)
    : prev(other.prev)
    , q(other.q)
    , f(std::move(other.f))
    , chained(other.chained)
  { other.chained = true; }

  ~TemplateDispatcher() noexcept(false) { if (!chained) wait_and_dispatch(); }

  TemplateDispatcher(const TemplateDispatcher&) = delete;
  TemplateDispatcher& operator=(const TemplateDispatcher&) = delete;

  template<typename OtherMsg, typename OtherFunc>
  auto // TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
  handle(OtherFunc&& of) {
    return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>\
    { this, q, std::forward<OtherFunc>(of) };
  }

private:
  template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
  friend class TemplateDispatcher;

  void wait_and_dispatch() {
    while (true) { if (dispatch(q->wait_and_pop())) break; }
  }

  bool dispatch(const message_shared& msg) {
    if (auto wrapper =\
      std::dynamic_pointer_cast<wrapped_message<Message>>(msg)) {
      f(wrapper->contents);
      return true;
    } else {
      return prev->dispatch(msg);
    }
  }

  queue*              q { nullptr };
  PreviousDispatcher* prev { nullptr };
  Func                f {};
  bool                chained { false };
};
} // end namespace messaging
