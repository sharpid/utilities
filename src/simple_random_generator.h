#pragma once
// simple static random number generator
#include <random>
#include <type_traits>

template <class engine_type = std::mt19937>
class SimpleRandomGenerator {
   public:
	SimpleRandomGenerator() : engine_(std::hash<int>()(std::random_device()())) {}
	// SimpleRandomGenerator() : engine_(std::random_device("/dev/random")()) {}
	SimpleRandomGenerator(unsigned int s)
		: engine_(std::hash<int>()(std::random_device()()) ^ (std::hash<int>()(s) << 1)) {}

   public:
	template <typename return_type>
	return_type Generate(return_type _min = 1, return_type _max = 10000);

   public:
	void SetSeed(unsigned int s) {
		engine_.seed(s);
	}
	void SetRandomSeed(unsigned int s = std::random_device()()) {
		auto seed = std::hash<int>()(std::random_device()()) ^ (std::hash<int>()(s) << 1);
		engine_.seed(seed);
	}

   private:
	template <typename return_type,
			  bool = std::is_integral<return_type>::value,
			  bool = std::is_floating_point<return_type>::value>
	struct __distributor__;

   private:
	engine_type engine_;
};

template <class engine_type>
template <typename return_type>
struct SimpleRandomGenerator<engine_type>::__distributor__<return_type, true, false> {
	// for integral number types
	inline return_type operator()(engine_type& _engine, const return_type& _min, const return_type& _max) {
		auto dist = std::uniform_int_distribution<return_type>(_min, _max);
		return dist(_engine);
	}
};

template <class engine_type>
template <typename return_type>
struct SimpleRandomGenerator<engine_type>::__distributor__<return_type, false, true> {
	// for floating point number types
	inline return_type operator()(engine_type& _engine, const return_type& _min, const return_type& _max) {
		auto dist = std::uniform_real_distribution<return_type>(_min, _max);
		return dist(_engine);
	}
};

template <class engine_type>
template <typename return_type>
inline return_type SimpleRandomGenerator<engine_type>::Generate(return_type _min, return_type _max) {
	if (_min == _max)
		return _min;

	// prevent exception
	if (_min > _max)
		std::swap(_min, _max);

	return __distributor__<return_type>()(engine_, _min, _max);
}
