#include <boost/format.hpp>
#include <boost/stacktrace.hpp>
#include "log_helper.h"

extern "C" {
#include "skynet.h"
}

//@todo, perfomance check,

namespace SG {
template <typename... Types>
inline void ConsoleLog(const LL _logging_level, const Types&... _args) {
	try {
		static const auto option_level = ResourceManager::get_const_instance().GetServerOptioon().logging_level;
		if (static_cast<int>(_logging_level) < option_level)
			return;

		std::string str_buffer;
		str_buffer.reserve(256);

		str_buffer += __FILE__;
		str_buffer += ":";
		str_buffer += std::to_string(__LINE__);
		str_buffer += " ";
		str_buffer += __FUNCTION_OUT__;
#ifdef _USE_PRETTY_FUNCTION_
#else
		str_buffer += "()";
#endif
		str_buffer += "\n";

		str_buffer += ToString(_args...);
		//__separate_log_arguments(str_buffer, _args...);

		switch (_logging_level) {
			case LL::Stack:
			case LL::Fatal:
				str_buffer += '\n';
				str_buffer += boost::stacktrace::to_string(boost::stacktrace::stacktrace());
				break;
			default:
				break;
		}

		switch (_logging_level) {
			case LL::Stack:
			case LL::Debug:
				LOG_DEBUG("%s", str_buffer.c_str());
				break;
			case LL::Info:
				LOG_INFO("%s", str_buffer.c_str());
				break;
			case LL::Warning:
				LOG_WARN("%s", str_buffer.c_str());
				break;
			case LL::Error:
				LOG_ERROR("%s", str_buffer.c_str());
				break;
			case LL::Fatal:
				LOG_ERROR("%s", str_buffer.c_str());
				break;
		}
	} catch (const std::exception& e) {
		LOG_ERROR("logging fail, %s", e.what());
		return;
	}
}

template <typename... Types>
std::string ToString(const Types&... _args) {
	std::string ret;
	ret.reserve(256);
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
class __to_string<google::protobuf::Message, false, false> {
   public:
	std::string operator()(const google::protobuf::Message& _v) { return _v.ShortDebugString(); }
	std::string operator()(const google::protobuf::Message* _v) { return _v->ShortDebugString(); }
};

template <>
class __to_string<std::string, false, false> {
   public:
	std::string operator()(const std::string& _v) { return _v; }
	std::string operator()(const std::string* _v) { return *_v; }
};

template <>
class __to_string<const char*, false, false> {
   public:
	std::string operator()(const char* const& _v) { return std::string(_v); }
};

template <>
class __to_string<std::ostringstream, false, false> {
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