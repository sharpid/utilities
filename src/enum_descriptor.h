#pragma once
#include <regex>
#include <string>
#include <type_traits>

template<typename T>
class EnumDescriptor
{
   public:
	template <typename V = std::underlying_type<T>::type>
	EnumDescriptor(const std::string& _str);

   public:
	bool IsValid(const T&) const;
	bool IsValid(const std::string&) const;

	//const std::string& GetString() const;
	const T* FindEnumByName(const std::string&) const;
	const std::string* FindNameByEnum(const T&) const;

	private:
	 std::map<std::string, T> name_to_enum_;
	 std::multimap<T, std::string> enum_to_name_;
};

template <typename T>
template <typename V>
EnumDescriptor<T>::EnumDescriptor(const std::string& _arg_str) {
	std::string arg;

	{
		//remove comments
		std::regex r_c_comment("(?:\\/\\*.*?\\*\\/)");
		std::regex r_cpp_comment("(?:\\/\\/.*)");
		arg = std::regex_replace(_arg_str, r_c_comment, "");
		arg = std::regex_replace(arg, r_cpp_comment, "");
	}

	std::regex r_token(",");
	std::regex r_enumerator("(\\w+)(?:[\\s]*=[\\s]*)?(\\d+)?");
	std::sregex_token_iterator iter(arg.begin(), arg.end(), r_token, -1);

	V enumerator_value = 0;

	while (iter != std::sregex_token_iterator()) {
		std::string str = iter->str();
		std::smatch r_result;

		if (std::regex_search(str, r_result, r_enumerator)) {
			const auto& enumerator_name = r_result[1];

			if (r_result[2].matched) {
				enumerator_value = std::stoi(r_result[2]);
			} else {
			}

			name_to_enum_.emplace(enumerator_name, static_cast<T>(enumerator_value));
			enum_to_name_.emplace(static_cast<T>(enumerator_value), enumerator_name);
		}

		++iter;

		++enumerator_value;
	}
}

template <typename T>
inline bool EnumDescriptor<T>::IsValid(const T& _rhs) const {
	return enum_to_name_.find(_rhs) != enum_to_name_.end();
}

template <typename T>
inline bool EnumDescriptor<T>::IsValid(const std::string& _name) const {
	return name_to_enum_.find(_name) != name_to_enum_;
}

template <typename T>
inline const T* EnumDescriptor<T>::FindEnumByName(const std::string& _name) const {

	auto it = name_to_enum_.find(_name);
	if (it != name_to_enum_.end())
		return &it->second;
	else
		return nullptr;
}

template <typename T>
inline const std::string* EnumDescriptor<T>::FindNameByEnum(const T& _enum) const {
	auto range = enum_to_name_.equal_range(_enum);
	if (range.first != range.second)
		return range.first->second;
	else
		return nullptr;
}

//
template <typename T> const EnumDescriptor<T>& GetEnumDescriptor();

#define EnumDefineWithType(NAME, TYPE, ... )\
        enum class NAME : TYPE{\
        __VA_ARGS__\
        };\
	static EnumDescriptor<NAME> NAME##_Descriptor(#__VA_ARGS__);\
	template <> static const EnumDescriptor<NAME>& GetEnumDescriptor<NAME>(){return NAME##_Descriptor; };

#define EnumDefine(NAME, ...) EnumDefineWithType(NAME, int32_t, __VA_ARGS__)



