#ifndef COR_UTILS_HPP
#define COR_UTILS_HPP

#include <mutex>
#include <string>
#include <cstddef>

namespace cor {
namespace utils {

std::string random_string(std::size_t length);
std::string get_filename(std::string file_path, bool with_extension = true, char separator = '/');

template <class T>
class Monitor
{
private:
	mutable T m_t;
	mutable std::mutex m_mtx;

public:
	using Type = T;
	Monitor() {}
	Monitor(T t_) : m_t(std::move(t_)) {}
	template <typename F>
	auto operator()(F f) const -> decltype(f(m_t))
	{
		std::lock_guard<std::mutex> hold{ m_mtx };
		return f(m_t);
	}
};

}}

#endif
