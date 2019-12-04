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

namespace SG {
// LoggingLevel
enum class LL : int {
	Stack = -1,
	Debug = 0,
	Info,
	Warning,
	Error,
	Fatal,
};

template <typename... Types>
void ConsoleLog(const LL, const Types&... _args);

template <typename... Types>
std::string ToString(const Types&... _args);

template <typename Iter>
std::string RangeToString(const Iter&, const Iter&);

// to string template functor
// not implement compile error
template <typename T,
		  bool = std::is_arithmetic<T>::value,
		  bool = std::is_base_of<LogInterface, typename std::remove_pointer<T>::type>::value,
		  typename... VA>
class __to_string;

template <typename T>
class __to_string<T, true, false>;

template <typename T>
class __to_string<T, false, true>;

template <>
class __to_string<google::protobuf::Message, false, false>;

template <>
class __to_string<std::string, false, false>;

template <>
class __to_string<const char*, false, false>;

template <>
class __to_string<std::ostringstream, false, false>;

//std container
template <typename... VA>
class __to_string<std::pair<VA...>, false, false>;

template <typename... VA>
class __to_string<std::tuple<VA...>, false, false>;

template <typename... VA>
class __to_string<std::vector<VA...>, false, false>;

template <typename... VA>
class __to_string<std::list<VA...>, false, false>;

template <typename... VA>
class __to_string<std::set<VA...>, false, false>;

template <typename... VA>
class __to_string<std::map<VA...>, false, false>;

template <typename... VA>
class __to_string<std::unordered_set<VA...>, false, false>;

template <typename... VA>
class __to_string<std::unordered_map<VA...>, false, false>;


// separate arguments
template <typename T, typename... Types>
void __separate_log_arguments(std::string& _buffer, const T& _arg, const Types&... _args);

template <typename T>
void __separate_log_arguments(std::string& _buffer, const T& _arg);

template <size_t N, typename... Types>
void __separate_log_arguments(std::string& _buffer, const char (&_arg)[N], const Types&... _args);

template <size_t N>
void __separate_log_arguments(std::string& _buffer, const char (&_arg)[N]);

}  // namespace SG
// member buffer logger
