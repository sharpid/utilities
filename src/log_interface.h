#pragma once
#include <stdio.h>
#include <boost/format.hpp>
#include <memory>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// interface
class LogInterface {
   public:
	virtual std::string ToString() const = 0;
	virtual std::string ToSimpleString() const = 0;
};


///////////////////////////////////////////////////////////////////////////////
// string formatting
template <typename... Types>
inline std::string string_format(const std::string& format, const Types&... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args...);
	return std::string(buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
}

inline std::string gametime_to_string(const float _time) {
	int seconds = int(_time) % 60;
	int minutes = int(_time) / 60;
	//	auto microseconds = int((_time - std::floor(_time)) * 1000000);
	auto milliseconds = int((_time - std::floor(_time)) * 1000);

	return string_format("%02d:%02d.%03d", minutes, seconds, milliseconds);
}
