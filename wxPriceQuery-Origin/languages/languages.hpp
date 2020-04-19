#pragma once
#include <map>
#include <wx/intl.h>

namespace languages
{
	class language_basic_class
	{
	protected:
		std::map<wxString, wxString> currency_full_name;
		std::map<wxString, wxString> two_letter_country_to_full_name;
		std::map<wxString, wxString> static_text_mapping;
	public:
		virtual wxString TranslateCurrencyName(wxString currency)
		{
			return currency_full_name[currency];
		}

		virtual wxString TranslateCountryCode(wxString two_letter_country_code)
		{
			return two_letter_country_to_full_name[two_letter_country_code];
		}

		virtual wxString TranslateStaticText(wxString text_code)
		{
			return static_text_mapping[text_code];
		}
	};

	enum class Language
	{
		English,
		ChineseS,
		ChineseT
	};

	std::map<Language, wxString> InterfaceLanguagesList();
	std::map<int, Language> EnumMappingList();
	wxString TranslateCurrencyName(Language lng, wxString currency);
	wxString TranslateCountryCode(Language lng, wxString two_letter_country_code);
	wxString TranslateStaticText(Language lng, wxString text_code);
}
