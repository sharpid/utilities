#pragma once
#include "log_helper.h"

//@todo, perfomance check,

///////////////////////////////////////////////////////////////////////////////
template <typename... Types>
std::string ToString(const Types&... _args) {
	StringHelper temp;
	return std::move(temp.ToString(_args...));
}

template <typename Iter>
std::string RangeToString(const Iter& _begin, const Iter& _end) {
	StringHelper temp;
	return temp.RangeToString(_begin, _end);
}

///////////////////////////////////////////////////////////////////////////////
template <typename... Types>
inline std::string StringHelper::ToString(const Types&... _args) {
	__separate_log_arguments(_args...);
	return buffer_;
}

template <typename Iter>
inline std::string StringHelper::RangeToString(const Iter& _begin, const Iter& _end) {
	buffer_ += "{";
	for (auto it = _begin; it != _end;) {
		buffer_ +=
			__to_string<typename std::remove_pointer<typename std::iterator_traits<Iter>::value_type>::type>()(*it);
		if (++it != _end)
			buffer_ += ",";
	}
	buffer_ += "}";

	return buffer_;
}

///////////////////////////////////////////////////////////////////////////////
// specialize
template <typename T>
struct StringHelper::__to_string<T, StringHelper::ARG_TYPE::ARITHMETIC> {
	inline std::string operator()(const T& _v) { return std::to_string(_v); }
	inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename T>
struct StringHelper::__to_string<T, StringHelper::ARG_TYPE::USER_LOG_INTERFACE> {
	inline std::string operator()(const T& _v) { return _v.ToString(); }
	inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename T>
struct StringHelper::__to_string<T, StringHelper::ARG_TYPE::GOOGLE_PROTO_ENUM> {
	inline std::string operator()(const T& _v) {
		return ::google::protobuf::internal::NameOfEnum(::google::protobuf::GetEnumDescriptor<T>(), _v);
	}
	inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename T>
struct StringHelper::__to_string<T, StringHelper::ARG_TYPE::GOOGLE_PROTO_MESSAGE> {
	inline std::string operator()(const T& _v) { return _v.ShortDebugString(); }
	inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename T>
struct StringHelper::__to_string<T, StringHelper::ARG_TYPE::STL_CONTAINER> {
	inline std::string operator()(const T& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::remove_pointer<typename T::value_type>::type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

// string and char
template <>
struct StringHelper::__to_string<std::string> {
	inline std::string operator()(const std::string& _v) { return _v; }
	inline std::string operator()(const std::string* _v) { return (!_v) ? "null" : operator()(*_v); };
};

template <>
struct StringHelper::__to_string<const char*> {
	inline std::string operator()(const char* const& _v) { return (!_v) ? "null" : std::string(_v); }
};

template <>
struct StringHelper::__to_string<std::ostringstream> {
	inline std::string operator()(const std::ostringstream& _v) { return _v.str(); }
	inline std::string operator()(const std::ostringstream* _v) { return (!_v) ? "null" : operator()(*_v); }
};

///////////////////////////////////////////////////////////////////////////////
// pair and tuple
template <typename... VA>
struct StringHelper::__to_string<std::pair<VA...>, StringHelper::ARG_TYPE::NONE> {
	inline std::string operator()(const std::pair<VA...>& _v) {
		// std::tuple_size<std::pair<VA...>>
		std::string ret;
		ret += "[";
		ret += __to_string<typename std::tuple_element<0, std::pair<VA...>>::type>()(_v.first);
		ret += ",";
		ret += __to_string<typename std::tuple_element<1, std::pair<VA...>>::type>()(_v.second);
		ret += "]";
		return ret;
	}
	inline std::string operator()(const std::pair<VA...>* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::tuple<VA...>, StringHelper::ARG_TYPE::NONE> {
	inline std::string operator()(const std::tuple<VA...>& _v) {
		std::string ret;
		ret += "[";
		const auto ts = std::tuple_size<std::tuple<VA...>>::value;
		ret += __tuple_decomposer__<ts - 1>()(_v);
		ret += "]";
		return ret;
	}
	inline std::string operator()(const std::tuple<VA...>* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <size_t SZ, typename... VA>
struct StringHelper::__tuple_decomposer__ {
	std::string operator()(std::tuple<VA...>& t) {
		return __tuple_decomposer__<SZ - 1>()(t) + "," +
			   __to_string<
				   typename std::remove_pointer<typename std::tuple_element<SZ, std::tuple<VA...>>::type>::type>()(
				   std::get<SZ>(t));
	}
};

template <typename... VA>
struct StringHelper::__tuple_decomposer__<0, VA...> {
	std::string operator()(std::tuple<VA...>& t) {
		return __to_string<typename std::remove_pointer<typename std::tuple_element<0, std::tuple<VA...>>::type>>::
			type()(std::get<0>(t));
	}
};

///////////////////////////////////////////////////////////////////////////////
// separate
template <typename T, typename... Types>
inline void StringHelper::__separate_log_arguments(const T& _arg, const Types&... _args) {
	buffer_ += __to_string<typename std::remove_pointer<T>::type>()(_arg);
	__separate_log_arguments(_args...);
}

template <typename T>
inline void StringHelper::__separate_log_arguments(const T& _arg) {
	buffer_ += __to_string<typename std::remove_pointer<T>::type>()(_arg);
}

template <size_t N, typename... Types>
inline void StringHelper::__separate_log_arguments(const char (&_arg)[N], const Types&... _args) {
	buffer_ += __to_string<const char*>()(_arg);
	__separate_log_arguments(_args...);
}

template <size_t N>
inline void StringHelper::__separate_log_arguments(const char (&_arg)[N]) {
	buffer_ += __to_string<const char*>()(_arg);
}
