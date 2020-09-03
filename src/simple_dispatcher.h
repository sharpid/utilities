#pragma once
#include <unordered_map>
#include <functional>

template< typename Ttype, typename Tret, typename ... Targs>
class Dispatcher
{
	using Handler = std::function<Tret(Targs...)>;
	using HandlerMap = std::unordered_map<Ttype, Handler>;

public:
	Dispatcher() = default;
	~Dispatcher() {
		handler_map_.clear();
	}

	void RegisterMessage(const Ttype& _type, const Handler& _handler) {
		handler_map_.emplace(_type, _handler);
	}

	virtual Tret HandleMessage(const Ttype& _type, const Targs& ... _args) {
		auto it = handler_map_.find(_type);
		if ( it != handler_map_.end() ) {
			return it->second(_args...);
		}

		return Tret();
	}

private:
	HandlerMap handler_map_;
};

