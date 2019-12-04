#pragma once
namespace SG {
class LogInterface {
   public:
	virtual std::string ToString() const = 0;
	virtual std::string ToSimpleString() const = 0;
};
}  // namespace SG