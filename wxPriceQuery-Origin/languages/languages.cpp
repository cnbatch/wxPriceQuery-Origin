#include <memory>
#include <typeinfo>
#include "languages.hpp"
#include "en.hpp"
#include "zh.hpp"

using namespace std;

namespace languages
{
	unique_ptr<language_basic_class> language_class_ptr;

	map<Language, wxString> language_names =
	{
		{Language::English, "English"},
		{Language::ChineseS, "中文（简体）"},
		{Language::ChineseT, "中文（繁體）"}
	};

	map<int, Language> language_enum_map =
	{
		{wxLANGUAGE_ENGLISH, Language::English},
		{wxLANGUAGE_ENGLISH_UK, Language::English},
		{wxLANGUAGE_ENGLISH_US, Language::English},
		{wxLANGUAGE_ENGLISH_AUSTRALIA, Language::English},
		{wxLANGUAGE_ENGLISH_BELIZE, Language::English},
		{wxLANGUAGE_ENGLISH_BOTSWANA, Language::English},
		{wxLANGUAGE_ENGLISH_CANADA, Language::English},
		{wxLANGUAGE_ENGLISH_CARIBBEAN, Language::English},
		{wxLANGUAGE_ENGLISH_DENMARK, Language::English},
		{wxLANGUAGE_ENGLISH_EIRE, Language::English},
		{wxLANGUAGE_ENGLISH_JAMAICA, Language::English},
		{wxLANGUAGE_ENGLISH_NEW_ZEALAND, Language::English},
		{wxLANGUAGE_ENGLISH_PHILIPPINES, Language::English},
		{wxLANGUAGE_ENGLISH_SOUTH_AFRICA, Language::English},
		{wxLANGUAGE_ENGLISH_TRINIDAD, Language::English},
		{wxLANGUAGE_ENGLISH_ZIMBABWE, Language::English},
		{wxLANGUAGE_CHINESE_SIMPLIFIED, Language::ChineseS},
		{wxLANGUAGE_CHINESE_TRADITIONAL, Language::ChineseT},
		{wxLANGUAGE_CHINESE_HONGKONG, Language::ChineseT},
		{wxLANGUAGE_CHINESE_MACAU, Language::ChineseT},
		{wxLANGUAGE_CHINESE_SINGAPORE, Language::ChineseS},
		{wxLANGUAGE_CHINESE_TAIWAN, Language::ChineseT},
		{wxLANGUAGE_CHINESE, Language::ChineseT}
	};

	void CheckClassPointer(Language lng);

	void CheckClassPointer(Language lng)
	{
		switch (lng)
		{
		case Language::English:
			if (language_class_ptr == nullptr || typeid(*language_class_ptr).hash_code() != typeid(language_english).hash_code())
				language_class_ptr = make_unique<language_english>();
			break;

		case Language::ChineseS:
			if (language_class_ptr == nullptr || typeid(*language_class_ptr).hash_code() != typeid(language_chinese_s).hash_code())
				language_class_ptr = make_unique<language_chinese_s>();
			break;

		case Language::ChineseT:
			if (language_class_ptr == nullptr || typeid(*language_class_ptr).hash_code() != typeid(language_chinese_t).hash_code())
				language_class_ptr = make_unique<language_chinese_t>();
			break;

		default:
			if (language_class_ptr == nullptr)
				language_class_ptr = make_unique<language_basic_class>();
			break;
		}
	}

	map<Language, wxString> InterfaceLanguagesList()
	{
		return language_names;
	}

	map<int, Language> EnumMappingList()
	{
		return language_enum_map;
	}

	wxString TranslateCurrencyName(Language lng, wxString currency)
	{
		CheckClassPointer(lng);
		return language_class_ptr->TranslateCurrencyName(currency);
	}

	wxString TranslateCountryCode(Language lng, wxString two_letter_country_code)
	{
		CheckClassPointer(lng);
		return language_class_ptr->TranslateCountryCode(two_letter_country_code);
	}

	wxString TranslateStaticText(Language lng, wxString text_code)
	{
		CheckClassPointer(lng);
		return language_class_ptr->TranslateStaticText(text_code);
	}

}

