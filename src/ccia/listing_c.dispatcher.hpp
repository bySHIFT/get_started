#pragma once
#include "listing_c.queue.hpp"

namespace messaging
{
class close_queue
{};

class dispatcher
{
private:
  template<typename Dispatcher, typename Msg, typename Func>
  friend class TemplateDispatcher;

public:
  dispatcher(dispatcher&& other)
    : q(other.q)
    , chained(other.chained)
  { other.chained = true; }

  explicit dispatcher(queue* q_) : q(q_), chained(false) {}

  template<typename Message,typename Func>
  TemplateDispatcher<dispatcher,Message,Func>
  handle(Func&& f)
  {
    return TemplateDispatcher<dispatcher,Message,Func>(q,this,std::forward<Func>(f));
  }

  ~dispatcher() noexcept(false)
  {
    if(!chained) wait_and_dispatch();
  }

private:
  queue* q { nullptr };
  bool chained { false };

  dispatcher(const dispatcher&) = delete;
  dispatcher& operator=(const dispatcher&) = delete;

  void wait_and_dispatch()
  {
    for (;;)
    {
      auto msg = q->wait_and_pop();
      dispatch(msg);
    }
  }

  bool dispatch(const std::shared_ptr<message_base>& msg)
  {
    if (dynamic_cast<wrapped_message<close_queue>*>(msg.get()))
    {
      throw close_queue();
    }

    return false;
  }
};
} // end namespace messaging
