#pragma once
#include "log_helper.h"

//@todo, perfomance check,

///////////////////////////////////////////////////////////////////////////////
template <typename... Types>
std::string ToString(const Types&... _args) {
	StringHelper temp;
	return temp.ToString(_args...);
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
		buffer_ += __to_string<typename std::iterator_traits<Iter>::value_type>()(*it);
		if (++it != _end)
			buffer_ += ",";
	}
	buffer_ += "}";

	return buffer_;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct StringHelper::__to_string<T, true, false> {
	inline std::string operator()(const T& _v) { return std::to_string(_v); }
	inline std::string operator()(const T* _v) { return operator()(*_v); }
};

template <typename T>
struct StringHelper::__to_string<T, false, true> {
	inline std::string operator()(const T& _v) { return _v.ToString(); }
	inline std::string operator()(const T* _v) { return operator()(*_v); }
};

template <>
struct StringHelper::__to_string<google::protobuf::Message> {
	inline std::string operator()(const google::protobuf::Message& _v) { return _v.ShortDebugString(); }
	inline std::string operator()(const google::protobuf::Message* _v) { return operator()(*_v); }
};

template <>
struct StringHelper::__to_string<std::string> {
	inline std::string operator()(const std::string& _v) { return _v; }
	inline std::string operator()(const std::string* _v) { return operator()(*_v); };
};

template <>
struct StringHelper::__to_string<const char*> {
	inline std::string operator()(const char* const& _v) { return std::string(_v); }
};

template <>
struct StringHelper::__to_string<std::ostringstream> {
	inline std::string operator()(const std::ostringstream& _v) { return _v.str(); }
	inline std::string operator()(const std::ostringstream* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::pair<VA...>, false, false> {
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
	inline std::string operator()(const std::pair<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::tuple<VA...>, false, false> {
	inline std::string operator()(const std::tuple<VA...>& _v) {
		std::string ret;
		const auto ts = std::tuple_size<std::tuple<VA...>>::value;
		ret += "[";
		for (int i = 0; i < ts;) {
			ret += __to_string<typename std::tuple_element<0, std::tuple<VA...>>::type>()(std::get<i>(_v));
			if (++i < ts)
				ret += ",";
		}
		ret += "]";
		return ret;
	}
	inline std::string operator()(const std::tuple<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::vector<VA...>, false, false> {
	inline std::string operator()(const std::vector<VA...>& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::vector<VA...>::value_type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const std::vector<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::list<VA...>, false, false> {
	inline std::string operator()(const std::list<VA...>& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::list<VA...>::value_type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const std::list<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::set<VA...>, false, false> {
	inline std::string operator()(const std::set<VA...>& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::set<VA...>::value_type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const std::set<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::map<VA...>, false, false> {
	inline std::string operator()(const std::map<VA...>& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::map<VA...>::value_type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const std::map<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::unordered_set<VA...>, false, false> {
	inline std::string operator()(const std::unordered_set<VA...>& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::unordered_set<VA...>::value_type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const std::unordered_set<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
struct StringHelper::__to_string<std::unordered_map<VA...>, false, false> {
	inline std::string operator()(const std::unordered_map<VA...>& _v) {
		std::string ret;
		ret += "{";
		for (auto it = _v.begin(); it != _v.end();) {
			ret += __to_string<typename std::unordered_map<VA...>::value_type>()(*it);
			if (++it != _v.end())
				ret += ",";
		}
		ret += "}";
		return ret;
	}
	inline std::string operator()(const std::unordered_map<VA...>* _v) { return operator()(*_v); }
};

///////////////////////////////////////////////////////////////////////////////
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
