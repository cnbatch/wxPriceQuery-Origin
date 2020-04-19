#pragma once
#include "languages.hpp"

namespace languages
{
	struct language_english : public language_basic_class
	{
		language_english()
		{
			currency_full_name =
			{
				{"USD", "US dollar"},
				{"JPY", "Japanese yen"},
				{"BGN", "Bulgarian lev"},
				{"CZK", "Czech koruna"},
				{"DKK", "Danish krone"},
				{"EUR", "Euro"},
				{"GBP", "Pound sterling"},
				{"HUF", "Hungarian forint"},
				{"PLN", "Polish zloty"},
				{"RON", "Romanian leu"},
				{"SEK", "Swedish krona"},
				{"CHF", "Swiss franc"},
				{"ISK", "Icelandic krona"},
				{"NOK", "Norwegian krone"},
				{"HRK", "Croatian kuna"},
				{"RUB", "Russian rouble"},
				{"TRY", "Turkish lira"},
				{"AUD", "Australian dollar"},
				{"BRL", "Brazilian real"},
				{"CAD", "Canadian dollar"},
				{"CNY", "Chinese yuan renminbi"},
				{"HKD", "Hong Kong dollar"},
				{"IDR", "Indonesian rupiah"},
				{"ILS", "Israeli shekel"},
				{"INR", "Indian rupee"},
				{"KRW", "South Korean won"},
				{"MXN", "Mexican peso"},
				{"MYR", "Malaysian ringgit"},
				{"NZD", "New Zealand dollar"},
				{"PHP", "Philippine peso"},
				{"SGD", "Singapore dollar"},
				{"THB", "Thai baht"},
				{"TWD", "Taiwan new dollar"},
				{"ZAR", "South African rand"}
			};

			two_letter_country_to_full_name =
			{
				{"AU", "Australia"},
				{"BE", "Belgium"},
				{"BR", "Brazil"},
				{"CA", "Canada"},
				{"DE", "Germany"},
				{"DK", "Denmark"},
				{"ES", "Spain"},
				{"FI", "Finland"},
				{"FR", "France"},
				{"GB", "United Kingdom"},
				{"HK", "Hong Kong"},
				{"IE", "Ireland"},
				{"IN", "India"},
				{"IT", "Italy"},
				{"JP", "Japan"},
				{"KR", "Korea"},
				{"MX", "Mexico"},
				{"NL", "Netherlands"},
				{"NO", "Norway"},
				{"NZ", "New Zealand"},
				{"PL", "Poland"},
				{"RU", "Russia"},
				{"SE", "Sweden"},
				{"SG", "Singapore"},
				{"TH", "Thailand"},
				{"TW", "Taiwan"},
				{"US", "USA"},
				{"ZA", "South Africa"}
			};

			static_text_mapping =
			{
				{"app_title", "Price Query Tool for Origin"},
				{"search", "Search"},
				{"price_sorting", "Price Sorting"},
				{"supported_languages", "Supported Languages"},
				{"settings", "Settings..."},
				{"interface_settings", "Interface Settings"},
				{"interface_language", "Interface Language"},
				{"my_currency", "My Currency"},
				{"origin_parameters", "Origin Parameters"},
				{"lng_game_list", "Language of Game List:"},
				{"region", "Region of Origin:"},
				{"ok_btn", "OK"},
				{"cancel_btn", "Cancel"},
				{"catalogue", "Catalogue"},
				{"title_name", "Title Name"},
				{"item_type", "Item Type"},
				{"platform", "Platform"},
				{"release_date", "Release Date"},
				{"on_sale", "On Sale"},
				{"wait", "Please wait"},
				{"download", "Downloading data from Origin Server"},
				{"connect", "Connecting to Origin Server"},
				{"cannot_connect", "Unable to connect Origin Server"},
				{"message", "Message"},
				{"conversion_failed", "conversion failed"},
				{"search_result", "Search Results"},
				{"all_games", "All Games"},
				{"current_region", "Current Region:"},
				{"other_region", "Other Regions:"},
				{"selection_missing", "Select an option"},
				{"select_currency", "Please select a Currency."},
				{"select_language", "Please select a Language."},
				{"select_region", "Please select a Region."},
				{"other_genre", "Other Genre"},
				{"other_franchise", "Other Franchise"},
				{"other_game_type", "Other Game Type"}
			};
		}
	};

}