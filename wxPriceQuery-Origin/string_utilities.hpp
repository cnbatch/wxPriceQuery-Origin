#pragma once
#include <string>

namespace string_utitlies
{
	void replace_string(std::string &str, const std::string &old_string, const std::string &new_string);
	void replace_string(std::wstring &str, const std::wstring &old_string, const std::wstring &new_string);

	void to_upper(std::string &str);
	std::string to_upper_copy(const std::string &str);

	void to_lower(std::string &str);
	std::string to_lower_copy(const std::string &str);

	void to_underline(std::string &str);
	std::string to_underline_copy(const std::string &str);

	void to_hyphen(std::string &str);
	std::string to_hyphen_copy(const std::string &str);

	void remove_trailing_zeros(std::string &number_string);
	void remove_trailing_zeros(std::wstring &number_string);
	std::string remove_trailing_zeros_copy(const std::string &number_string);
	std::wstring remove_trailing_zeros_copy(const std::wstring &number_string);
}

