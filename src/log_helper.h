#pragma once
#include <google/protobuf/message.h>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "log_interface.h"

#ifdef _USE_PRETTY_FUNCTION_
#if defined(_MSC_VER)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#define __FUNCTION_OUT__ __PRETTY_FUNCTION__
#else
#define __FUNCTION_OUT__ __func__
#endif

///////////////////////////////////////////////////////////////////////////////
// extern func
template <typename... Types>
std::string ToString(const Types&... _args);

template <typename Iter>
std::string RangeToString(const Iter&, const Iter&);

class StringHelper {
   public:
	StringHelper() { buffer_.reserve(reserve_buffer_size_); };

   public:
	template <typename... Types>
	std::string ToString(const Types&... _args);

	template <typename Iter>
	std::string RangeToString(const Iter&, const Iter&);

   private:
	const size_t reserve_buffer_size_ = 1024;
	std::string buffer_;

	///////////////////////////////////////////////////////////////////////////////
	// to string template functors
	template <typename T,
			  bool = std::is_arithmetic<T>::value,
			  bool = std::is_base_of<LogInterface, typename std::remove_pointer<T>::type>::value,
			  typename... VA>
	struct __to_string;

	///////////////////////////////////////////////////////////////////////////////
	// separate arguments
	template <typename T, typename... Types>
	void __separate_log_arguments(const T& _arg, const Types&... _args);
	template <typename T>
	void __separate_log_arguments(const T& _arg);

	template <size_t N, typename... Types>
	void __separate_log_arguments(const char (&_arg)[N], const Types&... _args);
	template <size_t N>
	void __separate_log_arguments(const char (&_arg)[N]);
};