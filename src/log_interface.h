#pragma once
class LogInterface {
   public:
	virtual std::string ToString() const = 0;
	virtual std::string ToSimpleString() const = 0;
};

#include <string>
#include <memory>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// string formatting
// @todo check performance vs boost::format
#include <boost/format.hpp>
template<typename ... Types>
inline std::string string_format(const std::string& format, const Types& ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[ size ]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
