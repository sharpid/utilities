#pragma once
#include <functional>
#include <map>
#include <stdexcept>
#include <unordered_map>

// Key must be hashable
template <typename T_TYPE, typename T_RET, typename... T_ARGS>
class MessageDispatcher {
	using T_FUNC = std::function<T_RET(T_ARGS...)>;
	using MessageMapT = std::unordered_map<T_TYPE, T_FUNC>;

   public:
	MessageDispatcher() = default;
	virtual ~MessageDispatcher() = default;

	void RegisterMessage(T_TYPE _type, T_FUNC _func);
	T_RET DispatchMessage(T_TYPE _type, T_ARGS... _args) const;

   private:
	MessageMapT message_map_;
};

template <typename T_TYPE, typename T_RET, typename... T_ARGS>
inline void MessageDispatcher<T_TYPE, T_RET, T_ARGS...>::RegisterMessage(
	T_TYPE _type,
	T_FUNC _func) {
	message_map_.emplace(_type, _func);
}

template <typename T_TYPE, typename T_RET, typename... T_ARGS>
inline T_RET MessageDispatcher<T_TYPE, T_RET, T_ARGS...>::DispatchMessage(
	T_TYPE _type,
	T_ARGS... _args) const {
	auto it = message_map_.find(_type);

	if (it != message_map_.end()) {
		return (*it).second(_args...);
	} else {
		throw std::runtime_error("dispatch fail");
	}
}

//void specialize, no throw
template <typename T_TYPE, typename... T_ARGS>
class MessageDispatcher<T_TYPE, void, T_ARGS...> {
	using T_FUNC = std::function<void(T_ARGS...)>;
	using MessageMapT = std::unordered_map<T_TYPE, T_FUNC>;

   public:
	MessageDispatcher() = default;
	virtual ~MessageDispatcher() = default;

	void RegisterMessage(T_TYPE _type, T_FUNC _func);
	void DispatchMessage(T_TYPE _type, T_ARGS... _args) const;

   private:
	MessageMapT message_map_;
};

template <typename T_TYPE, typename... T_ARGS>
inline void MessageDispatcher<T_TYPE, void, T_ARGS...>::RegisterMessage(
	T_TYPE _type,
	T_FUNC _func) {
	message_map_.emplace(_type, _func);
}

template <typename T_TYPE, typename... T_ARGS>
inline void MessageDispatcher<T_TYPE, void, T_ARGS...>::DispatchMessage(
	T_TYPE _type,
	T_ARGS... _args) const {
	auto it = message_map_.find(_type);

	if (it != message_map_.end()) {
		return (*it).second(_args...);
	}
}
