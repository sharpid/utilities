#pragma once
#include <chrono>

class BaseTimer
{
public:
	using TimepointT = std::chrono::time_point<std::chrono::system_clock>;

	//ctor
	BaseTimer() :start_point_(std::chrono::system_clock::now()) {}

public:
	template<typename T = float>
	const T Seconds() const
	{
		auto duration = GetNowPoint() - GetStartPoint();
		return std::chrono::duration_cast< std::chrono::duration<T, std::ratio<1, 1> > >( duration ).count();
	}

	template<typename T = float>
	const T MilliSeconds() const
	{
		auto duration = GetNowPoint() - GetStartPoint();
		return std::chrono::duration_cast< std::chrono::duration<T, std::milli> >( duration ).count();
	}

	template<typename T = float>
	const T MicroSeconds() const
	{
		auto duration = GetNowPoint() - GetStartPoint();
		return std::chrono::duration_cast< std::chrono::duration<T, std::micro> >( duration ).count();
	}

	template<typename T = float>
	const T NanoSeconds() const
	{
		auto duration = GetNowPoint() - GetStartPoint();
		return std::chrono::duration_cast< std::chrono::duration<T, std::nano> >( duration ).count();
	}

	template<typename T = float>
	const T Seconds(const TimepointT & _end_point) const
	{
		auto duration = GetNowPoint() - _end_point;
		return std::chrono::duration_cast< std::chrono::duration<T, std::ratio<1, 1> > >( duration ).count();
	}


	template<typename T = float>
	const T MilliSeconds(const TimepointT & _end_point) const
	{
		auto duration = GetNowPoint() - _end_point;
		return std::chrono::duration_cast< std::chrono::duration<T, std::milli> >( duration ).count();
	}

	template<typename T = float>
	const T MicroSeconds(const TimepointT & _end_point) const
	{
		auto duration = GetNowPoint() - _end_point;
		return std::chrono::duration_cast< std::chrono::duration<T, std::micro> >( duration ).count();
	}

	template<typename T = float>
	const T NanoSeconds(const TimepointT & _end_point) const
	{
		auto duration = GetNowPoint() - _end_point;
		return std::chrono::duration_cast< std::chrono::duration<T, std::nano> >( duration ).count();
	}

	TimepointT GetNowPoint()const
	{
		return std::chrono::system_clock::now();
	}

private:
	const TimepointT GetStartPoint() const
	{
		return start_point_;
	}

private:
	TimepointT start_point_;
};
