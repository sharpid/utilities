#pragma once
#include <unordered_map>
#include <stdexcept>
#include <functional>

template <typename T_TYPE, typename T_RET, typename ... T_ARGS >
class MessageDispatcher {
	using T_FUNC = std::function<T_RET(T_ARGS...)>;
	using MessageMapT = std::unordered_map<T_TYPE, T_FUNC>;
public:
	MessageDispatcher() = default;
	virtual ~MessageDispatcher() = default;

	void	RegisterMessage(const T_TYPE& _type, const T_FUNC& _func);
	T_RET	DispatchMessage(const T_TYPE& _type, const T_ARGS& ... _args) const;

private:
	MessageMapT message_map_;
};

template <typename T_TYPE, typename T_RET, typename ... T_ARGS >
inline void MessageDispatcher<T_TYPE, T_RET, T_ARGS...>::RegisterMessage(const T_TYPE& _type, const T_FUNC& _func) {
	message_map_.emplace(_type, _func);
}

template<typename T_TYPE, typename T_RET, typename ...T_ARGS>
inline T_RET MessageDispatcher<T_TYPE, T_RET, T_ARGS...>::DispatchMessage(const T_TYPE& _type, const T_ARGS& ... _args) const {
	auto it = message_map_.find(_type);

	if (it != message_map_.end()) {
		return ( *it ).second(_args ...);
	}
	else {
		throw std::runtime_error("dispatch fail");
	}
}

