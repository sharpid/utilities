#pragma once
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <google/protobuf/message.h>

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
// check container type,
template <typename T, typename _ = void>
struct is_container : std::false_type {};

template <typename... T>
struct is_container_helper {};

// SFINAE
template <typename T>
struct is_container<T,
					typename std::conditional<true,
											  void,
											  is_container_helper<typename T::iterator,
																  decltype(std::declval<T>().begin()),
																  decltype(std::declval<T>().end())> >::type>
	: public std::true_type {};

///////////////////////////////////////////////////////////////////////////////
// extern func
template <typename... Types>
std::string ToString(const Types&... _args);

template <typename Iter>
std::string RangeToString(const Iter&, const Iter&);

class StringHelper {
   public:
	enum ARG_TYPE {
		NONE = 0,  // not specialized
		ARITHMETIC,
		GOOGLE_PROTO_MESSAGE,
		GOOGLE_PROTO_ENUM,
		STL_CONTAINER,
		USER_LOG_INTERFACE,
	};

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
	template <typename T>
	constexpr static ARG_TYPE GetArgumentType() {
		return std::is_arithmetic<T>::value
				   ? ARG_TYPE::ARITHMETIC
				   : is_container<T>::value
						 ? ARG_TYPE::STL_CONTAINER
						 : std::is_base_of<LogInterface, typename std::remove_pointer<T>::type>::value
							   ? ARG_TYPE::USER_LOG_INTERFACE
							   : google::protobuf::is_proto_enum<typename std::remove_pointer<T>::type>::value
									 ? ARG_TYPE::GOOGLE_PROTO_ENUM
									 : std::is_convertible<typename std::add_pointer<T>::type,
														   const ::google::protobuf::Message*>::value
										   ? ARG_TYPE::GOOGLE_PROTO_MESSAGE
										   : ARG_TYPE::NONE;
	};

	template <typename T, ARG_TYPE = GetArgumentType<T>()>
	struct __to_string;

	///////////////////////////////////////////////////////////////////////////////
	// separate tuple arguments
	template <size_t SZ, typename... VA>
	struct __tuple_decomposer__;

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