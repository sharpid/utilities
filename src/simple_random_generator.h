#pragma once
// simple static random number generator
#include <random>
#include <type_traits>

class SimpleRandomGenerator {
	using engine_type = std::mt19937;

   public:
	SimpleRandomGenerator() : engine_(std::random_device()()) {}
	//SimpleRandomGenerator() : engine_(std::random_device("/dev/random")()) {}

   public:
	template <typename T>
	T Generate(T _min = 1, T _max = 10000);

   private:
	template <typename T, typename E, bool = std::is_integral<T>::value, bool = std::is_floating_point<T>::value>
	struct __distributor__;

   private:
	engine_type engine_;
};

template <typename T, typename E>
struct SimpleRandomGenerator::__distributor__<T, E, true, false> {
	// for integral number types
	inline T operator()(E& _engine, const T& _min, const T& _max) {
		auto dist = std::uniform_int_distribution<T>(_min, _max);
		return dist(_engine);
	}
};

template <typename T, typename E>
struct SimpleRandomGenerator::__distributor__<T, E, false, true> {
	// for floating point number types
	inline T operator()(E& _engine, const T& _min, const T& _max) {
		auto dist = std::uniform_real_distribution<T>(_min, _max);
		return dist(_engine);
	}
};

template <typename T>
inline T SimpleRandomGenerator::Generate(T _min, T _max) {
	if (_min == _max)
		return _min;

	// prevent exception
	if (_min > _max)
		std::swap(_min, _max);

	return __distributor__<T, engine_type>()(engine_, _min, _max);
}
