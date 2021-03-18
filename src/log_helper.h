#pragma once
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <memory>

#ifdef _USE_GOOGLE_PROTOBUF_
#include <google/protobuf/message.h>
#endif  //_USE_GOOGLE_PROTOBUF_

#include "log_interface.h"

#ifdef _USE_PRETTY_FUNCTION_
  #if defined(_MSC_VER)
    #define __FUNCTION_OUT__ __FUNCSIG__
  #else
    #define __FUNCTION_OUT__ __PRETTY_FUNCTION__
  #endif
#else
  #define __FUNCTION_OUT__ __func__
#endif

template <typename... T>
struct template_helper {};

///////////////////////////////////////////////////////////////////////////////
// check container type,
template <typename T, typename _ = void>
struct is_container : std::false_type {};
// SFINAE
template <typename T>
struct is_container<
    T,
    typename std::conditional<
        true,
        void,
        template_helper<typename T::iterator,
                        decltype(std::declval<T>().begin()),
                        decltype(std::declval<T>().end())>>::type>
    : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////
// check include custom to_string
template <typename T, typename _ = void>
struct has_camel_tostring : std::false_type {};

template <typename T, typename _ = void>
struct has_snake_tostring : std::false_type {};

template <typename T>
struct has_camel_tostring<
    T,
    typename std::conditional<
        true,
        void,
        template_helper<decltype(std::declval<T>().ToString())>>::type>
    : public std::true_type {};

template <typename T>
struct has_snake_tostring<
    T,
    typename std::conditional<
        true,
        void,
        template_helper<decltype(std::declval<T>().to_string())>>::type>
    : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////

class StringHelper {
 public:
  enum ARG_TYPE {
    NONE = 0,
    ARITHMETIC,
    STL_CONTAINER,
    USER_LOG_INTERFACE,
    HAS_CAMEL_TOSTRING,
    HAS_SNAKE_TOSTRING,

#ifdef _USE_GOOGLE_PROTOBUF_
    GOOGLE_PROTO_ENUM,
    GOOGLE_PROTO_MESSAGE,
#endif  //_USE_GOOGLE_PROTOBUF_

  };

 public:
  StringHelper():buffer_(std::make_shared<std::string>()) { buffer_->reserve(reserve_buffer_size_); };
  StringHelper(std::shared_ptr<std::string> _buffer):buffer_(_buffer) { buffer_->reserve(reserve_buffer_size_); };

 public:
  template <typename... Types>
  std::string ToString(const Types&... _args);

  template <typename Iter>
  std::string RangeToString(const Iter&, const Iter&);

 private:
  const size_t reserve_buffer_size_ = 1024;
  std::shared_ptr<std::string> buffer_;

  ///////////////////////////////////////////////////////////////////////////////
  // to string template functors
  /*
  template <typename T>
  constexpr static ARG_TYPE GetArgumentType() {
    using refT = typename std::remove_pointer<T>::type;
    using ptrT = typename std::add_pointer<T>::type;

    if (std::is_arithmetic<T>::value)
      return ARG_TYPE::ARITHMETIC;
    if (is_container<T>::value)
      return ARG_TYPE::STL_CONTAINER;
    if (std::is_base_of<LogInterface, refT>::value)
      return ARG_TYPE::USER_LOG_INTERFACE;

#ifdef _USE_GOOGLE_PROTOBUF_
    if (google::protobuf::is_proto_enum<refT>::value)
      return ARG_TYPE::GOOGLE_PROTO_ENUM;
    if (std::is_convertible<ptrT, const ::google::protobuf::Message*>::value)
      return ARG_TYPE::GOOGLE_PROTO_MESSAGE;
#endif// _USE_GOOGLE_PROTOBUF_

    return ARG_TYPE::NONE;
  };
  */
  // clang-format off
  template <typename T>
  constexpr static ARG_TYPE GetArgumentType() {
    return std::is_arithmetic<T>::value ? ARG_TYPE::ARITHMETIC
		 : is_container<T>::value ? ARG_TYPE::STL_CONTAINER
		 : std::is_base_of< LogInterface, typename std::remove_pointer<T>::type>::value ? ARG_TYPE::USER_LOG_INTERFACE
	     : has_camel_tostring<T>::value ? ARG_TYPE::HAS_CAMEL_TOSTRING
		 : has_snake_tostring<T>::value ? ARG_TYPE::HAS_SNAKE_TOSTRING
#ifdef _USE_GOOGLE_PROTOBUF_
	     : google::protobuf::is_proto_enum<typename std::remove_pointer<T>::type>::value ? ARG_TYPE::GOOGLE_PROTO_ENUM
		 : std::is_convertible<typename std::add_pointer<T>::type, const ::google::protobuf::Message*>:: value ? ARG_TYPE::GOOGLE_PROTO_MESSAGE
#endif
	   : ARG_TYPE::NONE;
  };
  // clang-format on

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