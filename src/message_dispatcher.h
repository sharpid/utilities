#pragma once
#include <unordered_map>

template <typename T_TYPE, typename T_FUNC, typename ... T_ARGS >
class MessageDispatcher
{
	typedef std::unordered_map<T_TYPE, T_FUNC> MessageMapT;
public:
	MessageDispatcher() = default;
	virtual ~MessageDispatcher() { message_map_.clear(); }

	void RegisterMessage(T_TYPE _type, T_FUNC _func);
	bool DispatchMessage(T_TYPE _type, T_ARGS...);

private:
	MessageMapT message_map_;
};

template<typename T_TYPE, typename T_FUNC, typename ...T_ARGS>
inline void MessageDispatcher<T_TYPE, T_FUNC, T_ARGS...>::RegisterMessage(T_TYPE _type, T_FUNC _func)
{
	message_map_.emplace(_type, _func);
}

template<typename T_TYPE, typename T_FUNC, typename ...T_ARGS>
inline bool MessageDispatcher<T_TYPE, T_FUNC, T_ARGS...>::DispatchMessage(T_TYPE _type, T_ARGS ... _args)
{
	auto it = message_map_.find(_type);

	if (it != message_map_.end())
	{
		return ( *it ).second(_args ...);
	}
	else
	{
		return false;
	}
}

