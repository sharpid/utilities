#pragma once

#include <string>
#include <sstream>
#include <memory>

//@todo perfomance is not good, need to optimize
//@todo auth
template <typename T>
class formula_helper
{
////////////////////////////////////////////////////////////////
// ctor
public:
	formula_helper() = default;
	formula_helper(const T& _v) :value_(_v)
	{}
	formula_helper(const T& _v, std::string _name) :value_(_v), name_(_name)
	{}

	formula_helper(std::shared_ptr<formula_helper> _lhs, const std::string _op, std::shared_ptr<formula_helper>(_rhs))
		:left_ptr_(_lhs), operator_(_op), right_ptr_(_rhs)
	{
		if (operator_ == "+")
			value_ = left_ptr_->value_ + right_ptr_->value_;
		else if (operator_ == "-")
			value_ = left_ptr_->value_ - right_ptr_->value_;
		else if (operator_ == "*")
			value_ = left_ptr_->value_ * right_ptr_->value_;
		else if (operator_ == "/")
			value_ = left_ptr_->value_ / right_ptr_->value_;
		else
			value_ = left_ptr_->value_;
	}

	formula_helper(std::shared_ptr<formula_helper> _lhs, const std::string _op)
		:left_ptr_(_lhs), operator_(_op)
	{
		if (operator_ == "abs")
			value_ = std::abs(left_ptr_->value_);
		else
			value_ = left_ptr_->value_;
	}



////////////////////////////////////////////////////////////////
// public function
public:
	std::string DebugString() const
	{
		if (operator_.empty())
		{
			return GetValueString();
		}
		else
		{
			auto temp = std::to_string(value_);
			temp += "=";
			temp += GetSubString();

			return temp;
		}
	}

	T GetValue() const
	{
		return value_;
	}



private:
	std::string GetValueString() const
	{
		return ( name_.empty() ? "" : name_ + ":" ) + std::to_string(value_);
	}

protected:
	std::string GetSubString() const
	{
		if (operator_.empty())
		{
			return GetValueString();
		}

		auto temp = left_ptr_->GetSubString();

		auto IsBinaryOperator = [ & ]() -> bool
		{
			return operator_ == "+" ||
				operator_ == "-" ||
				operator_ == "*" ||
				operator_ == "/";
		};
		if (IsBinaryOperator())
		{
			temp += operator_;
			if (right_ptr_ != nullptr)
				temp += right_ptr_->GetSubString();
		}
		else
		{
			//uanary
			if (operator_ =="bracket")
				temp = "(" + temp + ")";
			else
				temp = operator_ + "(" + temp + ")";
		}

		return temp;
	}


////////////////////////////////////////////////////////////////
// binary operators
public:
#define REGISTE_OPERATOR(OPERATOR) \
	formula_helper operator OPERATOR (const formula_helper& _rhs) const\
	{\
		return formula_helper(std::make_shared<formula_helper>(*this), #OPERATOR , std::make_shared<formula_helper>(_rhs));\
	}\
	formula_helper operator OPERATOR (const float& _rhs) const\
	{\
		return *this OPERATOR formula_helper(_rhs);\
	}

	REGISTE_OPERATOR(+);
	REGISTE_OPERATOR(-);
	REGISTE_OPERATOR(*);
	REGISTE_OPERATOR(/);
#undef REGISTE_OPERATOR


////////////////////////////////////////////////////////////////
// unary operators
public:
	// @todo , add unary functions, ex)abs ...
	formula_helper abs() const
	{
		return formula_helper(std::make_shared<formula_helper>(*this), "abs");
	}

	formula_helper bracket() const
	{
		return formula_helper(std::make_shared<formula_helper>(*this), "bracket");
	}

	formula_helper operator () () const
	{
		return bracket();
	}

////////////////////////////////////////////////////////////////
// cast
public:
	operator T() const
	{
		return GetValue();
	}

	operator std::string() const
	{
		return DebugString();
	}



////////////////////////////////////////////////////////////////
// member
private:
	T								value_ = 0.f;
	std::string						name_;

	std::shared_ptr<formula_helper>	left_ptr_ = nullptr;
	std::string						operator_;
	std::shared_ptr<formula_helper>	right_ptr_ = nullptr;
};


#define REGISTE_OPERATOR(OPERATOR) \
template<typename T>\
formula_helper<T> operator OPERATOR (const T& _lhs, const formula_helper<T>& _rhs)\
{\
	return formula_helper<T>(_lhs) OPERATOR _rhs;\
}

REGISTE_OPERATOR(+)
REGISTE_OPERATOR(-)
REGISTE_OPERATOR(*)
REGISTE_OPERATOR(/)

#undef REGISTE_OPERATOR

//#define FHI		formula_helper<int>
#define FHF		formula_helper<float>
//#define FHL		formula_helper<long>
//#define FHLL	formula_helper<long long>
