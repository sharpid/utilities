#pragma once
// simple static random number generator
#include <random>
#include <type_traits>

class SimpleRandomGenerator {
	using engine_type = std::mt19937;

   public:
	SimpleRandomGenerator() : engine_(std::random_device()()) {}
	// SimpleRandomGenerator() : engine_(std::random_device("/dev/random")()) {}

   public:
	template <typename T>
	T Generate(const T& _min = 1, const T& _max = 10000);

   private:
	template <typename T, bool = std::is_integral<T>::value, bool = std::is_floating_point<T>::value>
	struct __distributor__;

   private:
	engine_type engine_;
};

// integral number types
template <typename T>
struct SimpleRandomGenerator::__distributor__<T, true, false> {
	inline T operator()(engine_type& _engine, const T& _min, const T& _max) {
		auto dist = std::uniform_int_distribution<T>(_min, _max);
		return dist(_engine);
	}
};

// floating point number types
template <typename T>
struct SimpleRandomGenerator::__distributor__<T, false, true> {
	inline T operator()(engine_type& _engine, const T& _min, const T& _max) {
		auto dist = std::uniform_real_distribution<T>(_min, _max);
		return dist(_engine);
	}
};

template <typename T>
inline T SimpleRandomGenerator::Generate(const T& _min, const T& _max) {
	if (_min == _max)
		return _min;

	if (_min > _max)
		return __distributor__<T>()(engine_, _max, _min);
	else
		return __distributor__<T>()(engine_, _min, _max);
}
