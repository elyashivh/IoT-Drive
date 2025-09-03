#ifndef __ILRD_PQUEUE_HPP__
#define __ILRD_PQUEUE_HPP__

#include <queue>

namespace ilrd_166_7
{

	/**
	 * @class PQueue
	 * @brief A thin wrapper around std::priority_queue providing simplified access.
	 *
	 * A templated priority queue implementation with customizable comparison logic and container type.
	 * This wrapper exposes a minimal subset of operations: front, push, pop, and empty.
	 *
	 * @tparam T Type of elements stored in the queue.
	 * @tparam Container Underlying container used by std::priority_queue (default: std::vector<T>).
	 * @tparam Compare Comparison functor to determine priority (default: std::less<T>).
	 */
	template <typename T,
			  typename Container = std::vector<T>,
			  typename Comapre = std::less<typename Container::value_type>>
	class PQueue
	{
	public:
		const T &front() const
		{
			return m_pq.top();
		}
		void pop()
		{
			m_pq.pop();
		}
		void push(const T &data)
		{
			m_pq.push(data);
		}
		bool empty() const
		{
			return m_pq.empty();
		}

	private:
		std::priority_queue<T, Container, Comapre> m_pq;
	}; // PQueue
} // namespace ilrd_166_7
#endif // __ILRD_PQUEUE_HPP__
