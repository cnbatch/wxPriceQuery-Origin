#include "string_utilities.hpp"
#include <algorithm>
#include <cctype>

namespace string_utitlies
{
	void replace_string(std::string &str, const std::string &old_string, const std::string &new_string)
	{
		if (old_string.empty())
			return;
		for (std::string::size_type start_pos = str.find(old_string); start_pos != std::string::npos; start_pos = str.find(old_string, start_pos + new_string.length()))
		{
			str.replace(start_pos, old_string.length(), new_string);
		}
	}

	void replace_string(std::wstring &str, const std::wstring &old_string, const std::wstring &new_string)
	{
		if (old_string.empty())
			return;
		for (std::wstring::size_type start_pos = str.find(old_string); start_pos != std::wstring::npos; start_pos = str.find(old_string, start_pos + new_string.length()))
		{
			str.replace(start_pos, old_string.length(), new_string);
		}
	}

	void to_upper(std::string &str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char ch) { return std::toupper(ch); });
	}

	std::string to_upper_copy(const std::string &str)
	{
		std::string new_str = str;
		std::transform(new_str.begin(), new_str.end(), new_str.begin(),
			[](unsigned char ch) { return std::toupper(ch); });
		return new_str;
	}

	void to_lower(std::string &str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char ch) { return std::tolower(ch); });
	}

	std::string to_lower_copy(const std::string &str)
	{
		std::string new_str = str;
		std::transform(new_str.begin(), new_str.end(), new_str.begin(),
			[](unsigned char ch) { return std::tolower(ch); });
		return new_str;
	}

	void to_underline(std::string &str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char ch) { return ch == '-' ? '_' : ch; });
	}

	std::string to_underline_copy(const std::string &str)
	{
		std::string new_str = str;
		std::transform(new_str.begin(), new_str.end(), new_str.begin(),
			[](unsigned char ch) { return ch == '-' ? '_' : ch; });
		return new_str;
	}

	void to_hyphen(std::string &str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char ch) { return ch == '_' ? '-' : ch; });
	}

	std::string to_hyphen_copy(const std::string &str)
	{
		std::string new_str = str;
		std::transform(new_str.begin(), new_str.end(), new_str.begin(),
			[](unsigned char ch) { return ch == '_' ? '-' : ch; });
		return new_str;
	}

	void remove_trailing_zeros(std::string &number_string)
	{
		if (number_string.find('.') != number_string.npos)
		{
			number_string = number_string.substr(0, number_string.find_last_not_of('0') + 1);
			if ('.' == number_string[number_string.length() - 1])
			{
				number_string = number_string.substr(0, number_string.length() - 1);
			}
		}
	}

	void remove_trailing_zeros(std::wstring &number_string)
	{
		if (number_string.find(L'.') != number_string.npos)
		{
			number_string = number_string.substr(0, number_string.find_last_not_of(L'0') + 1);
			if ('.' == number_string[number_string.length() - 1])
			{
				number_string = number_string.substr(0, number_string.length() - 1);
			}
		}
	}

	std::string remove_trailing_zeros_copy(const std::string &number_string)
	{
		if (number_string.find('.') != number_string.npos)
		{
			std::string str = number_string.substr(0, number_string.find_last_not_of('0') + 1);
			if ('.' == str[str.length() - 1])
			{
				return str.substr(0, str.length() - 1);
			}
			else return str;
		}
		else return number_string;
	}

	std::wstring remove_trailing_zeros_copy(const std::wstring &number_string)
	{
		if (number_string.find('.') != number_string.npos)
		{
			std::wstring str = number_string.substr(0, number_string.find_last_not_of(L'0') + 1);
			if (L'.' == str[str.length() - 1])
			{
				return str.substr(0, str.length() - 1);
			}
			else return str;
		}
		else return number_string;
	}

}
