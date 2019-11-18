#ifndef INCLUDE_TASK_HXX__
#define INCLUDE_TASK_HXX__
#include <functional>
#include <type_traits>

namespace zh
{
template<typename T>
class task;

template<typename ResultType, typename... ArgTypes>
class task<ResultType(ArgTypes...)>
{
public:
  using TaskFunctionTypes = std::function<ResultType(ArgTypes...)>;

public:
  task(TaskFunctionTypes&& fn) : m_fnTask(std::move(fn)) {}
  task(TaskFunctionTypes& fn) : m_fnTask(fn) {}

  ResultType run(ArgTypes&&... args) {
    return m_fnTask(std::forward<ArgTypes>(args)...);
  }

  template<typename Callable>
  auto then(Callable&& fn)                                             \
/*  ->task<typename std::result_of<Callable(ResultType)>::type(ArgTypes...)> */
  {
    return task<std::result_of_t<Callable(ResultType)>(ArgTypes...)>   \
      ([this, &fn](ArgTypes&&... args)
// 这个后置类型推导必须要有
#if 1
      -> std::result_of_t<Callable(ResultType)>
#else
      -> std::result_of_t<std::decay_t<Callable>(std::decay_t<ResultType>)>
#endif
      { return fn(m_fnTask(std::forward<ArgTypes>(args)...)); });
  }

private:
  TaskFunctionTypes m_fnTask;
};
} // end namespace zh

#endif // INCLUDE_TASK_HXX__
