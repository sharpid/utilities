#pragma once
#include <functional>
#include <tuple>
#include <type_traits>

namespace hash_helper {
template <typename T, bool is_enum = std::is_enum<T>::value>
struct _hash {
	std::size_t operator()(const T& _t) const { return std::hash<T>()(_t); }
};

// specialize for std c++11 enum
template <typename T>
struct _hash<T, true> {
	using convert_t = typename std::underlying_type<T>::type;
	std::size_t operator()(const T& _t) const { return std::hash<convert_t>()(static_cast<convert_t>(_t)); }
};

///////////////////////////////////////////////////////////////////////////////
// for pair, tuple ...
// from boost container hash
inline size_t hash_combine_impl(size_t seed, size_t value) {
	return seed ^ (value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template <typename T>
inline size_t hash_combine(size_t seed, T const& v) {
	_hash<T> hasher;
	return hash_combine_impl(seed, hasher(v));
}

///////////////////////////////////////////////////////////////////////////////
//
template <size_t N, typename... VA>
struct __tuple_hash_impl__;

template <size_t N, typename... VA>
struct __tuple_hash_impl__ {
	size_t operator()(const std::tuple<VA...>& t) const {
		using current_type = typename std::tuple_element<N, std::tuple<VA...>>::type;
		_hash<current_type> hasher;
		return hash_helper::hash_combine(__tuple_hash_impl__<N - 1, VA...>()(t), hasher(std::get<N>(t)));
	}
};

template <typename... VA>
struct __tuple_hash_impl__<0, VA...> {
	size_t operator()(const std::tuple<VA...>& t) const {
		using current_type = typename std::tuple_element<0, std::tuple<VA...>>::type;
		_hash<current_type> hasher;
		return hasher(std::get<0>(t));
	}
};
}  // namespace hash_helper

namespace std {
// specialize for pair
template <typename T1, typename T2>
struct hash<std::pair<T1, T2>> {
	size_t operator()(const std::pair<T1, T2>& pair) const {
		std::size_t seed = 0;
		hash_helper::hash_combine(seed, pair.first);
		hash_helper::hash_combine(seed, pair.second);
		return seed;
	}
};
// specialize for tuple
template <typename... VA>
struct hash<std::tuple<VA...>> {
	size_t operator()(const std::tuple<VA...>& t) const {
		const size_t ts = std::tuple_size<const std::tuple<VA...>>::value - 1;
		return hash_helper::__tuple_hash_impl__<ts, VA...>()(t);
	}
};
}  // namespace std

