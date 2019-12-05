#include <boost/format.hpp>
#include <boost/stacktrace.hpp>
#include "log_helper.h"

//@todo, perfomance check,

namespace SG {
const uint32_t reserved_buffer_size = 1024;
template <typename... Types>
std::string ToString(const Types&... _args) {
	std::string ret;
	ret.reserve(reserved_buffer_size);
	__separate_log_arguments(ret, _args...);
	return ret;
}

template <typename Iter>
std::string RangeToString(const Iter& _begin, const Iter& _end) {
	std::string ret;
	ret += "{";
	for (auto it = _begin; it != _end;) {
		ret += __to_string<typename std::iterator_traits<Iter>::value_type>()(*it);
		if (++it != _end)
			ret += ",";
	}
	ret += "}";

	return ret;
}

template <typename T>
class __to_string<T, true, false> {
   public:
	std::string operator()(const T& _v) { return std::to_string(_v); }
	std::string operator()(const T* _v) { return std::to_string(*_v); }
};

template <typename T>
class __to_string<T, false, true> {
   public:
	std::string operator()(const T& _v) { return _v.ToString(); }
	std::string operator()(const T* _v) { return _v->ToString(); }
};

template <>
class __to_string<google::protobuf::Message>{
   public:
	std::string operator()(const google::protobuf::Message& _v) { return _v.ShortDebugString(); }
	std::string operator()(const google::protobuf::Message* _v) { return _v->ShortDebugString(); }
};

template <>
class __to_string<std::string>{
   public:
	std::string operator()(const std::string& _v) { return _v; }
	std::string operator()(const std::string* _v) { return *_v; }
};

template <>
class __to_string<const char*>{
   public:
	std::string operator()(const char* const& _v) { return std::string(_v); }
};

template <>
class __to_string<std::ostringstream>{
   public:
	std::string operator()(const std::ostringstream& _v) { return _v.str(); }
	std::string operator()(const std::ostringstream* _v) { return _v->str(); }
};

template <typename... VA>
class __to_string<std::pair<VA...>, false, false> {
   public:
	std::string operator()(const std::pair<VA...>& _v) {
		// std::tuple_size<std::pair<VA...>>
		std::string ret;
		ret += "[";
		ret += __to_string<typename std::tuple_element<0, std::pair<VA...>>::type>()(_v.first);
		ret += ",";
		ret += __to_string<typename std::tuple_element<1, std::pair<VA...>>::type>()(_v.second);
		ret += "]";
		return ret;
	}
	std::string operator()(const std::pair<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::tuple<VA...>, false, false> {
   public:
	std::string operator()(const std::tuple<VA...>& _v) {
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
	std::string operator()(const std::tuple<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::vector<VA...>, false, false> {
   public:
	std::string operator()(const std::vector<VA...>& _v) {
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
	std::string operator()(const std::vector<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::list<VA...>, false, false> {
   public:
	std::string operator()(const std::list<VA...>& _v) {
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
	std::string operator()(const std::list<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::set<VA...>, false, false> {
   public:
	std::string operator()(const std::set<VA...>& _v) {
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
	std::string operator()(const std::set<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::map<VA...>, false, false> {
   public:
	std::string operator()(const std::map<VA...>& _v) {
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
	std::string operator()(const std::map<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::unordered_set<VA...>, false, false> {
   public:
	std::string operator()(const std::unordered_set<VA...>& _v) {
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
	std::string operator()(const std::unordered_set<VA...>* _v) { return operator()(*_v); }
};

template <typename... VA>
class __to_string<std::unordered_map<VA...>, false, false> {
   public:
	std::string operator()(const std::unordered_map<VA...>& _v) {
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
	std::string operator()(const std::unordered_map<VA...>* _v) { return operator()(*_v); }
};

template <typename T, typename... Types>
inline void __separate_log_arguments(std::string& _buffer, const T& _arg, const Types&... _args) {
	_buffer += __to_string<typename std::remove_pointer<T>::type>()(_arg);
	__separate_log_arguments(_buffer, _args...);
}

template <typename T>
inline void __separate_log_arguments(std::string& _buffer, const T& _arg) {
	_buffer += __to_string<typename std::remove_pointer<T>::type>()(_arg);
}

template <size_t N, typename... Types>
inline void __separate_log_arguments(std::string& _buffer, const char (&_arg)[N], const Types&... _args) {
	_buffer += __to_string<const char*>()(_arg);
	__separate_log_arguments(_buffer, _args...);
}

template <size_t N>
inline void __separate_log_arguments(std::string& _buffer, const char (&_arg)[N]) {
	_buffer += __to_string<const char*>()(_arg);
}
}  // namespace SG