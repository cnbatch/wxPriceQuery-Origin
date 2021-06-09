#pragma once
#include <wx/event.h>
#include <wx/string.h>
#include <vector>
#include <map>
#include <tuple>
#include <future>
#include <unordered_set>

namespace query_tools
{
	struct GameDetailData
	{
		wxString offer_id;
		wxString offer_type;	// Use it when origin_display_type is "None"
		wxString offer_path;	// use for match discount json
		wxString game_type_facet_key;
		wxString origin_display_type;
		std::map<wxString, wxString> extra_display_type;	// if available, ignore origin_display_type; <language_code_underline, extra_display_type>
		std::vector<wxString> supported_languages;	// Use translated language
		std::map<wxString, wxString> game_platforms_and_release_date;
		std::map<wxString, wxString> item_name;	// language code, name
		std::map<wxString, wxString> display_name;	// language code, name
		std::map<wxString, double> price_normal;	// currency, price
		std::map<wxString, double> price_discounted;	// currency, price
		std::unordered_set<wxString> avaliable_country_two_letter;
	};

	class OriginQueries
	{
	private:
		bool cancel_web_request;
		int time_zone_hours;
		wxEvtHandler *window_handler;
		std::atomic<int> running_sessions;

		const std::string origin_url = "https://www.origin.com/";
		const std::string origin_base_api = "https://api{num}.origin.com/";
		const std::string origin_data_api = "https://data{num}.origin.com/";
		const std::string origin_supercat_full_json = "supercat/{country2letter}/{locale}/supercat-PCWIN_MAC-{country2letter}-{locale}.json.gz";
		const std::string origin_search_onsale_json = "xsearch/store/{locale}/{threeLetterCountry}/products?searchTerm=&filterQuery=price%3Aon-sale&sort=rank%20desc&start={start_pos}&rows=30&isGDP=true";
		const std::string origin_shell_translation = "defaults/web-defaults/defaults.{countrylocale}.config";
		const std::string origin_check_offer_link = "supercarp/rating/offers/anonymous?country={country2letter}&ifSubscriber=false&locale={locale}&currency={currency}&offerIds=";
		const std::string origin_check_single_offer = "ecommerce2/public/supercat/{offerid}/{locale}?country={country2letter}";
		const std::string user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0";

		std::string origin_index_url;
		std::string origin_index_html;
		std::string origin_locale_json_url;

		std::map<wxString, wxString> two_letter_country_map_to_currency_name;
		std::map<wxString, wxString> three_letter_country_map_to_currency_symbol;
		std::map<wxString, wxString> two_letter_country_map_to_three_letter_country;
		std::map<wxString, wxString> game_supported_language_code_hyphen_map_to_local_display;
		std::map<wxString, wxString> game_supported_language_code_underline_map_to_local_display;
		std::map<wxString, wxString> language_code_hyphen_map_to_display_language;

		std::map<std::string, std::shared_ptr<GameDetailData>> offer_id_map_to_game_detail;
		std::map<std::string, std::unordered_set<std::string>> offer_path_map_to_offer_ids;
		// genreFacetKey / franchiseFacetKey / gameNameFacetKey , Games (offer id)
		std::map<std::string, std::map<std::string, std::map<std::string, std::unordered_set<std::string>>>> catalogue_game_mapping_offer_id;
		std::map<std::string, std::map<std::string, std::map<std::string, std::unordered_set<std::string>>>> catalogue_game_mapping_offer_path;
		std::map<std::string, std::map<std::string, std::string>> keyword_translations;

		std::unordered_set<std::string> downloaded_language_code;

		std::string ISO8601ToLocalTime(const std::string &date_string);

		std::string GenerateNumberInString(int min_num, int max_num);
		std::string GetJsonString(std::string expression_string, const std::string &content);

		wxString GetFileContentFromURL(wxString url, int retry = 3);
		std::vector<wxString> GetFileContentFromURLs(const std::vector<wxString> &urls, int retry = 3);
		void RetrieveCountryProfile(std::function<void(std::string newmsg)> update_progress);
		void RetrieveCurrencyNameList(const wxString &file_content);
		void RetrieveCurrencySymbolList();
		void RetrieveGameSupportedLanguageMapping(const wxString &file_content);
		void TwoLetterCountryToThreeLetterCountry(const wxString &file_content);
		void RetrieveDisplayLanguageList();
		std::string RetreiveGameInfo(wxString two_letter_country_code, wxString language_code_underline);
		std::vector<std::string> RetrieveOnSaleList(wxString two_letter_country_code, wxString language_code_underline);
		std::string GetDiscountURL(const std::string two_letter_country_code, const std::string language_code_underline, const std::string currency, const std::string offer_id);
		std::map<std::string, std::string> QueryDiscounts(std::map<std::string, std::string> urls);
		void UnpackSupercatData(const std::map<std::string, std::string> &two_letter_country_map_to_game_info, const std::map<std::string, std::vector<std::string>> &two_letter_country_map_to_discount_list, const std::string &language_code_underline, std::map<std::string, std::vector<std::string>> &two_letter_country_map_to_discount_offer_id_list);
		void ConvertBWCurrency(const std::string &two_letter_country_code, const std::string &bioware_virtual_currency_offer_id, const std::vector<std::string> &offer_ids_with_bw);
		void UpdateDiscountInfo(const std::string &two_letter_country_code, const std::string &offer_id, const std::string &response_string);
		void RetrieveTranslation(const std::string &three_letter_country_code, const std::string &origin_display_language_code);
		void InitialTimeSettings();

	public:
		OriginQueries();
		~OriginQueries();
		OriginQueries & operator=(const OriginQueries&) = delete;
		OriginQueries(const OriginQueries&) = delete;
		OriginQueries(wxEvtHandler *current_window_handler);

		void SetWindowHandler(wxEvtHandler *current_window_handler);
		void CancelWebRequest();

		std::tuple<bool, std::string> ConnectOriginServer();
		std::map<wxString, wxString> GetDisplayLanguageList(std::function<void(std::string newmsg)> update_progress);
		std::map<wxString, wxString> GetDisplayLanguageList(std::function<void(std::string newmsg)> update_progress, std::function<void()> end_up)
		{
			auto return_map = GetDisplayLanguageList(update_progress);
			end_up();
			return return_map;
		}
		std::map<wxString, wxString> GetCurrencyList() { return two_letter_country_map_to_currency_name; }

		void CurrentLocation();	// https://api2.origin.com/atom/ipGeoLocation

		void DownloadGameList(std::string two_letter_country_code, std::string origin_display_language_code, std::function<void(std::string newmsg)> update_progress);
		std::string KeywordTranslation(const std::string &origin_display_language_code, const std::string &keyword);

		std::map<std::string, std::shared_ptr<GameDetailData>> GetOfferIDMapping() { return offer_id_map_to_game_detail; };
		std::map<std::string, std::unordered_set<std::string>> GetOfferIdFromOfferPathMap() { return offer_path_map_to_offer_ids; };
		std::map<std::string, std::map<std::string, std::map<std::string, std::unordered_set<std::string>>>> GetCatalogueMappingOfferID() { return catalogue_game_mapping_offer_id; };
		std::map<std::string, std::map<std::string, std::map<std::string, std::unordered_set<std::string>>>> GetCatalogueMappingOfferPath() { return catalogue_game_mapping_offer_path; };
	};
}
