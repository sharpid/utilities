#pragma once
#include <regex>
#include <string>
#include <type_traits>

template <typename T>
class EnumDescriptor {
   public:
	template <typename V = std::underlying_type<T>::type>
	EnumDescriptor(const std::string&, const std::string&);

   public:
	bool IsValid(const T&) const;
	bool IsValid(const std::string&) const;

	const T* FindEnumByString(const std::string&) const;
	std::string GetString(const T&) const;
	std::string GetFullString(const T&) const;

   private:
	std::string base_name_;
	std::map<std::string, T> name_to_enum_;
	std::multimap<T, std::string> enum_to_name_;
};

template <typename T>
template <typename V>
EnumDescriptor<T>::EnumDescriptor(const std::string& _base_name, const std::string& _arg_str) : base_name_(_base_name) {
	std::string arg;
	// remove comments
	std::regex r_c_comment("(?:\\/\\*.*?\\*\\/)");
	std::regex r_cpp_comment("(?:\\/\\/.*)");
	arg = std::regex_replace(_arg_str, r_c_comment, "");
	arg = std::regex_replace(arg, r_cpp_comment, "");

	//tokenize
	std::regex r_token(",");
	std::sregex_token_iterator iter(arg.begin(), arg.end(), r_token, -1);

	//find enumerator and value
	std::regex r_enumerator("(\\w+)(?:[\\s]*=[\\s]*(\\d+))?");

	V enumerator_value = 0;

	while (iter != std::sregex_token_iterator()) {
		std::string str = iter->str();
		std::smatch r_result;

		// find enumerator and value
		// regex result [1] : enumerator name
		//              [2] : enumerator value ( = 1 )?
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
	return name_to_enum_.find(_name) != name_to_enum_.end();
}

template <typename T>
inline const T* EnumDescriptor<T>::FindEnumByString(const std::string& _name) const {
	auto it = name_to_enum_.find(_name);
	if (it != name_to_enum_.end())
		return &it->second;
	else
		return nullptr;
}

template <typename T>
inline std::string EnumDescriptor<T>::GetString(const T& _enum) const {
	auto range = enum_to_name_.equal_range(_enum);
	if (range.first != range.second)
		return range.first->second;
	else
		return std::string();
}

template <typename T>
inline std::string EnumDescriptor<T>::GetFullString(const T& _enum) const {
	auto range = enum_to_name_.equal_range(_enum);
	if (range.first != range.second)
		return base_name_ + "::" + range.first->second;
	else
		return std::string();
}

//
template <typename T>
const EnumDescriptor<T>& GetEnumDescriptor();

#define EnumDefineWithType(NAME, TYPE, ...)                               \
	enum class NAME : TYPE { __VA_ARGS__ };                               \
	static EnumDescriptor<NAME> NAME##_Descriptor(#NAME, #__VA_ARGS__);   \
	template <>                                                           \
	static const EnumDescriptor<NAME>& GetEnumDescriptor<NAME>() {        \
		return NAME##_Descriptor;                                         \
	};

#define EnumDefine(NAME, ...) EnumDefineWithType(NAME, int32_t, __VA_ARGS__)
