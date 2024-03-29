#include "OriginQueries.hpp"
#include <wx/webrequest.h>
#include <wx/msgdlg.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <random>
#include <regex>
#include <future>
#include <nlohmann/json.hpp>
#ifdef __WXMSW__
#include <rapidxml/rapidxml.hpp>
#else
#include <rapidxml.h>
#endif
#include "../../string_utilities.hpp"
#include <wx/file.h>

namespace query_tools
{
	using namespace std;
	using nlohmann::json;
	using namespace string_utitlies;
	using namespace rapidxml;

	wxString OriginQueries::GetFileContentFromURL(wxString url, int retry)
	{
		wxString trim_url = url;
		trim_url.Trim();

		wxString file_content;
		wxWebRequest web_request;
		wxWebSession web_session = wxWebSession::New();
		if (web_session.IsOpened())
			web_request = web_session.CreateRequest(window_handler, trim_url);
		else
			web_request = wxWebSession::GetDefault().CreateRequest(window_handler, trim_url);

		if (!web_request.IsOk())
			return file_content;

		web_request.SetHeader("User-Agent", user_agent);

		if (cancel_web_request)
			return file_content;

		running_sessions++;

		web_request.Start();
		while (web_request.GetState() == wxWebRequest::State_Active)
		{
			if (cancel_web_request)
			{
				web_request.Cancel();
				running_sessions--;
				return file_content;
			}
			this_thread::sleep_for(100ms);
		}

		running_sessions--;
		//wxFile output("log.log", wxFile::write_append);
		//output.Write("URL: " + url + " | retry: " + to_string(retry) + "\r\n");
		//output.Close();
		if (file_content = web_request.GetResponse().AsString(); file_content.IsEmpty())
		{
			//wxFile output("log.log", wxFile::write_append);
			//output.Write("URL: " + url + " | file_content size(): " + to_string(file_content.size()) + "\r\n");
			//output.Close();
			if (0 != retry)
				file_content = GetFileContentFromURL(url, retry - 1);
		}

		return file_content;
	}

	vector<wxString> OriginQueries::GetFileContentFromURLs(const vector<wxString> &urls, int retry)
	{
		vector<wxString> results;

		for (auto &url : urls)
			results.emplace_back(GetFileContentFromURL(url, retry));

		return results;
	}

	string OriginQueries::ISO8601ToLocalTime(const string &date_string)
	{
		tm time_struct;
		istringstream time_sstream(date_string);
		time_sstream >> get_time(&time_struct, "%Y-%m-%dT%H:%M:%SZ");
		time_struct.tm_hour += time_zone_hours;
		time_t utc_time = mktime(&time_struct);
		char current_time[64]{};
		strftime(current_time, sizeof(current_time), "%F", localtime(&utc_time));
		string local_time_str(current_time);

		return local_time_str;
	}

	string OriginQueries::GenerateNumberInString(int min_num, int max_num)
	{
		random_device rd;
		uniform_int_distribution<int> distribution_number(min_num, max_num);
		return to_string(distribution_number(rd));
	}

	string OriginQueries::GetJsonString(string expression_string, const string &content)
	{
		regex filter_json_exp(expression_string, std::regex_constants::icase);
		smatch match_filter;
		if (regex_search(content, match_filter, filter_json_exp))
		{
			string json_string = match_filter.str();
			if (auto first_char = json_string.find_first_of("{"); first_char != 0 && first_char != string::npos)
			{
				json_string.erase(0, first_char);
			}

			return json_string;
		}
		else return string();
	}

	OriginQueries::OriginQueries() : cancel_web_request(false), window_handler(nullptr)
	{
		InitialTimeSettings();
	}

	OriginQueries::~OriginQueries()
	{
	}

	OriginQueries::OriginQueries(wxEvtHandler *current_window_handler)
		: cancel_web_request(false), window_handler(current_window_handler)
	{
		InitialTimeSettings();
	}

	void OriginQueries::SetWindowHandler(wxEvtHandler *current_window_handler)
	{
		window_handler = current_window_handler;
	}

	void OriginQueries::CancelWebRequest()
	{
		cancel_web_request = true;
		while (running_sessions != 0)
			this_thread::sleep_for(500ms);
		cancel_web_request = false;
	}

	tuple<bool, string> OriginQueries::ConnectOriginServer()
	{
		wxWebSession web_session = wxWebSession::New();
		wxWebRequest web_request;
		if (web_session.IsOpened())
			web_request = web_session.CreateRequest(window_handler, origin_url);
		else
			web_request = wxWebSession::GetDefault().CreateRequest(window_handler, origin_url);

		web_request.SetHeader("User-Agent", user_agent);

		if (!web_request.IsOk())
			return { false, string() };

		web_request.Start();
		while (web_request.GetState() == wxWebRequest::State_Active)
		{
			this_thread::sleep_for(100ms);
		}

		origin_index_html = web_request.GetResponse().AsString().ToStdString();
		origin_index_url = web_request.GetResponse().GetURL().ToStdString();

		return { true, string() };
	}

	void OriginQueries::RetrieveCountryProfile(std::function<void(std::string newmsg)> update_progress)
	{
		if (origin_index_html.empty())
			return;

		vector<future<void>> set_value_handles;
		vector<future<wxString>> get_value_handles;

		set_value_handles.emplace_back(async(launch::async, bind(&OriginQueries::RetrieveDisplayLanguageList, this)));
		set_value_handles.emplace_back(async(launch::async, bind(&OriginQueries::RetrieveCurrencySymbolList, this)));

		regex expression(R"(<script[^>]*src=".*?\.js"></script>)", std::regex_constants::icase);
		smatch matches;
		for (auto text = origin_index_html; regex_search(text, matches, expression); text = matches.suffix())
		{
			string html_js_label = matches.str();
			auto pos = html_js_label.find(".js");
			auto start_pos = html_js_label.rfind('\"', pos) + 1;
			auto end_pos = html_js_label.find('\"', pos);
			auto partial_url = html_js_label.substr(start_pos, end_pos - start_pos);
			if ('/' == partial_url[0])
			{
				partial_url.erase(0, 1);
			}
			wxString js_file_path = origin_url + partial_url;
			get_value_handles.emplace_back(async(launch::async, bind(&OriginQueries::GetFileContentFromURL, this, placeholders::_1, placeholders::_2), js_file_path, retry_times));
		}

		update_progress("access_origin");

		wxString file_content;
		for (auto &handle : get_value_handles)
		{
			file_content += handle.get();
		}

		if (!file_content.empty())
		{
			set_value_handles.emplace_back(async(launch::async, bind(&OriginQueries::TwoLetterCountryToThreeLetterCountry, this, placeholders::_1), reference_wrapper(file_content)));
			set_value_handles.emplace_back(async(launch::async, bind(&OriginQueries::RetrieveCurrencyNameList, this, placeholders::_1), reference_wrapper(file_content)));
			set_value_handles.emplace_back(async(launch::async, bind(&OriginQueries::RetrieveGameSupportedLanguageMapping, this, placeholders::_1), reference_wrapper(file_content)));
		}

		update_progress("get_regional");

		for (auto &handle : set_value_handles)
		{
			handle.wait();
		}
	}

	void OriginQueries::RetrieveCurrencyNameList(const wxString &file_content)
	{
		auto currency_start = file_content.find("{currency:\"");
		auto start = file_content.rfind("=", currency_start);
		auto to_be_match = file_content.substr(start);
		string json_string = GetJsonString(R"(=\{..:(?:\{currency.*?:.*?,.*?ratingsystem.*?\}.*?).*?\}\})", to_be_match.utf8_string());
		if (!json_string.empty())
		{
			replace_string(json_string, "{", "{\"");
			replace_string(json_string, ",", ",\"");
			replace_string(json_string, ":", "\":");

			try
			{
				json country_letter_json = json::parse(json_string);
				for (auto &[key, value] : country_letter_json.items())
				{
					string key_str = key;
					if (to_upper_copy(key_str) == "DEFAULT") continue;
					string value_str = value["currency"];
					two_letter_country_map_to_currency_name[to_upper_copy(key_str)] = wxString::FromUTF8(to_upper_copy(value_str));
				}
			}
			catch (json::exception &ex)
			{
				wxMessageBox(ex.what(), __FUNCTION__);
			}
		}
	}

	void OriginQueries::RetrieveCurrencySymbolList()
	{
		if (auto pos = origin_index_html.find("currency.json"); pos != string::npos)
		{
			auto start_pos = origin_index_html.rfind('}', pos) + 1;
			auto end_pos = origin_index_html.find('\"', pos);

			wxString json_file_path = origin_data_api + origin_index_html.substr(start_pos, end_pos - start_pos);
			json_file_path.Replace("{num}", GenerateNumberInString(1, 4));

			wxString file_content = GetFileContentFromURL(json_file_path);
			if (file_content.empty())
			{
				return;
			}

			try
			{
				json currency_json = json::parse(file_content.utf8_string());
				for (auto &[key, value] : currency_json["country"].items())
				{
					string country_code = key;
					string symbol_str = value["symbol"];
					three_letter_country_map_to_currency_symbol[to_upper_copy(country_code)] = wxString::FromUTF8(symbol_str);
				}
			}
			catch (json::exception &ex)
			{
				wxMessageBox(ex.what(), __FUNCTION__);
			}
		}
	}

	void OriginQueries::RetrieveGameSupportedLanguageMapping(const wxString &file_content)
	{
		auto position = file_content.find("en_WW");
		if (position == string::npos) return;
		auto start = file_content.rfind("{", position);
		auto end = file_content.find("}", position);
		auto json_string = file_content.substr(start, end - start + 1);
		if (!json_string.IsEmpty())
		{
			//replace_string(json_string, "{", "{\"");
			//replace_string(json_string, ",", ",\"");
			//replace_string(json_string, ":", "\":");
			json_string.Replace("{", "{\"");
			json_string.Replace(",", ",\"");
			json_string.Replace(":", "\":");

			try
			{
				json country_letter_json = json::parse(json_string.utf8_string());
				for (auto &[key, value] : country_letter_json.items())
				{
					string key_str = key;
					string value_str = value;
					game_supported_language_code_underline_map_to_local_display[key_str] = wxString::FromUTF8(value_str);
					replace(key_str.begin(), key_str.end(), '_', '-');
					game_supported_language_code_hyphen_map_to_local_display[key_str] = wxString::FromUTF8(value_str);
				}
			}
			catch (json::exception &ex)
			{
				wxMessageBox(ex.what(), __FUNCTION__);
			}
			catch (...)
			{
				wxMessageBox(__FUNCTION__);
			}
		}
	}

	void OriginQueries::TwoLetterCountryToThreeLetterCountry(const wxString &file_content)
	{
		string json_string = GetJsonString(R"(a\=\{us\:.*?\})", file_content.utf8_string());
		if (!json_string.empty())
		{
			replace_string(json_string, "{", "{\"");
			replace_string(json_string, ",", ",\"");
			replace_string(json_string, ":", "\":");

			try
			{
				json country_letter_json = json::parse(json_string);
				for (auto &[key, value] : country_letter_json.items())
				{
					string key_str = key;
					string value_str = value;
					two_letter_country_map_to_three_letter_country[to_upper_copy(key_str)] = wxString::FromUTF8(to_upper_copy(value_str));
				}
			}
			catch (json::exception &ex)
			{
				wxMessageBox(ex.what(), __FUNCTION__);
			}
		}
	}

	void OriginQueries::RetrieveDisplayLanguageList()
	{
		if (auto pos = origin_index_html.find("locale.json"); pos != string::npos)
		{
			auto start_pos = origin_index_html.rfind('}', pos) + 1;
			auto end_pos = origin_index_html.find('\"', pos);

			wxString json_file_path = origin_data_api + origin_index_html.substr(start_pos, end_pos - start_pos);
			json_file_path.Replace("{num}", GenerateNumberInString(1, 4));
			wxString file_content = GetFileContentFromURL(json_file_path);

			if (file_content.IsEmpty())
			{
				return;
			}

			try
			{
				json locale_json = json::parse(file_content.utf8_string());
				for (auto &[key, value] : locale_json.items())
				{
					string key_str = key;
					string value_str = value;
					language_code_hyphen_map_to_display_language[key_str] = wxString::FromUTF8(value_str);
				}
			}
			catch (json::exception &ex)
			{
				wxMessageBox(ex.what(), __FUNCTION__);
			}
		}
	}

	std::map<wxString, wxString> OriginQueries::GetDisplayLanguageList(std::function<void(std::string newmsg)> update_progress)
	{
		if (language_code_hyphen_map_to_display_language.empty())
		{
			RetrieveCountryProfile(update_progress);
		}

		return language_code_hyphen_map_to_display_language;
	}

	string OriginQueries::RetreiveGameInfo(wxString two_letter_country_code, wxString language_code_underline)
	{
		wxString lng_code = language_code_underline;
		if (lng_code.Contains("en_"))
		{
			lng_code[3] = two_letter_country_code[0];
			lng_code[4] = two_letter_country_code[1];
		}
		wxString supercat_full_url = origin_base_api + origin_supercat_full_json;
		supercat_full_url.Replace("{num}", GenerateNumberInString(1, 4));
		supercat_full_url.Replace("{country2letter}", two_letter_country_code);
		supercat_full_url.Replace("{locale}", lng_code);

		return GetFileContentFromURL(supercat_full_url).utf8_string();
	}

	string OriginQueries::KeywordTranslation(const string &origin_display_language_code, const std::string &keyword)
	{
		string keyword_to_match = keyword;
		replace(keyword_to_match.begin(), keyword_to_match.end(), ' ', '-');
		if (auto it = keyword_translations[origin_display_language_code].find(to_lower_copy(keyword_to_match));
			it != keyword_translations[origin_display_language_code].end())
		{
			return it->second;
		}
		else
		{
			return keyword;
		}
	}

	string OriginQueries::GetDiscountURL(const string two_letter_country_code, const string language_code_underline, const string currency, const string offer_id)
	{
		wxString price_query_url = origin_base_api + origin_check_offer_link + offer_id;
		price_query_url.Replace("{num}", GenerateNumberInString(1, 4));
		price_query_url.Replace("{country2letter}", two_letter_country_code);
		price_query_url.Replace("{locale}", language_code_underline);
		price_query_url.Replace("{currency}", currency);
		return price_query_url.utf8_string();
	}

	map<string, string> OriginQueries::QueryDiscounts(map<string, string> urls)
	{
		map<string, string> results;

		for (auto &[offer_id, url] : urls)
		{
			wxString response = GetFileContentFromURL(url);
			results[offer_id] = response.utf8_string();
		}

		return results;
	}

	void OriginQueries::UnpackSupercatData(const map<string, string> &two_letter_country_map_to_game_info, const map<string, vector<string>> &two_letter_country_map_to_discount_list, const std::string &language_code_underline, map<string, vector<string>> &two_letter_country_map_to_discount_offer_id_list)
	{
		for (auto &[two_letter_country_code, game_info] : two_letter_country_map_to_game_info)
		{
			if (const string &file_content = game_info; !file_content.empty())
			{
				json json_file;
				try
				{
					json_file = json::parse(file_content);
				}
				catch (...)
				{
					if (file_content.find(R"(xml version="1.0")") != string::npos && file_content.find("AccessDenied") != string::npos)
					{}
					else wxMessageBox(__FUNCTION__);
					continue;
				}

				if (json &offer_array = json_file["offers"]; offer_array.is_array())
				{
					string bioware_virtual_currency_offer_id;
					vector<string> offer_ids_with_bw;

					auto discount_list = two_letter_country_map_to_discount_list.at(two_letter_country_code);
					for (json &offer : offer_array)
					{
						string offer_id = offer["offerId"];
						string offer_path;
						shared_ptr<GameDetailData> offer_detail_data;
						if (offer_id_map_to_game_detail.find(offer_id) == offer_id_map_to_game_detail.end())
						{
							offer_detail_data = make_shared<GameDetailData>();
							offer_id_map_to_game_detail[offer_id] = offer_detail_data;

							offer_detail_data->offer_id = offer_id;
							offer_detail_data->offer_type = offer["offerType"].get<string>();
							offer_detail_data->offer_path = offer["offerPath"].is_null() ? "" : offer["offerPath"].get<string>();

							offer_detail_data->game_type_facet_key = offer["gameTypeFacetKey"].is_null() ? "" : offer["gameTypeFacetKey"].get<string>();
							offer_detail_data->origin_display_type = offer["originDisplayType"].is_null() ? "" : offer["originDisplayType"].get<string>();

							for (auto &[key, value] : offer["softwareLocales"].items())
							{
								string language_code = value;
								string local_language_name = game_supported_language_code_underline_map_to_local_display[language_code].ToStdString();
								offer_detail_data->supported_languages.push_back(local_language_name);
							}

							for (auto &[key, value] : offer["platforms"].items())
							{
								string os = value["platform"];
								offer_detail_data->game_platforms_and_release_date[os] = value["releaseDate"].is_null() ? "" : ISO8601ToLocalTime(value["releaseDate"]);
							}
						}
						else
						{
							offer_detail_data = offer_id_map_to_game_detail[offer_id];
						}

						offer_detail_data->extra_display_type[language_code_underline] = offer["i18n"]["extraContentDisplayGroupDisplayName"].is_null() ?
							"" : wxString::FromUTF8(offer["i18n"]["extraContentDisplayGroupDisplayName"]);

						wxString item_name = wxString::FromUTF8(offer["itemName"].get<string>());
						offer_detail_data->item_name[language_code_underline] = item_name;

						wxString display_name = offer["i18n"]["displayName"].is_null() ?
							item_name : wxString::FromUTF8(offer["i18n"]["displayName"].get<string>());
						offer_detail_data->display_name[language_code_underline] = display_name;
						if (display_name.find("1600") != string::npos && to_lower_copy(offer["developerFacetKey"].get<string>()) == "bioware")
							bioware_virtual_currency_offer_id = offer_id;

						string currency;
						double price = 0;
						if (!offer["countries"]["countryCurrency"].is_null())
						{
							currency = offer["countries"]["countryCurrency"];
							if (currency == "_BW")
								offer_ids_with_bw.push_back(offer_id);
							price = offer["countries"]["catalogPrice"].is_null() ? 0 : atof(offer["countries"]["catalogPrice"].get<string>().c_str());
						}
						if (offer["countries"]["countryCurrency"].is_null() || currency != two_letter_country_map_to_currency_name[two_letter_country_code])
						{
							if (currency != "_BW")
							{
								if (!offer["countries"]["countryCurrencyA"].is_null() && offer["countries"]["countryCurrencyA"].size() != 0)
								{
									for (size_t i = 0; i < offer["countries"]["countryCurrencyA"].size(); ++i)
									{
										if (offer["countries"]["countryCurrencyA"][i].get<string>() == two_letter_country_map_to_currency_name[two_letter_country_code])
										{
											currency = offer["countries"]["countryCurrencyA"][i].get<string>();
											price = atof(offer["countries"]["catalogPriceA"][i].get<string>().c_str());
											break;
										}
									}
								}
								else
								{
									currency = two_letter_country_map_to_currency_name[two_letter_country_code];
									price = 0;
								}
							}
						}
						if (currency.empty()) currency = two_letter_country_map_to_currency_name[two_letter_country_code];
						offer_detail_data->price_normal[currency] = price;

						if (!offer["offerPath"].is_null())
						{
							offer_path = offer["offerPath"].get<string>();
							offer_path_map_to_offer_ids[offer_path].insert(offer_id);
							if (!discount_list.empty() && find(discount_list.begin(), discount_list.end(), offer_path) != discount_list.end())
							{
								two_letter_country_map_to_discount_offer_id_list[two_letter_country_code].push_back(offer_id);
							}
						}

						offer_detail_data->avaliable_country_two_letter.insert(two_letter_country_code);

						string genreFacetKey = "Other Genre";
						string franchiseFacetKey = "Other Franchise";
						string gameNameFacetKey = "Other Game Type";
						regex comma("\\,+");

						if (!offer["genreFacetKey"].is_null())
						{
							genreFacetKey = offer["genreFacetKey"].get<string>();
						}

						if (!offer["franchiseFacetKey"].is_null())
						{
							franchiseFacetKey = offer["franchiseFacetKey"].get<string>();
						}

						if (!offer["gameNameFacetKey"].is_null())
						{
							gameNameFacetKey = offer["gameNameFacetKey"].get<string>();
						}
						if (!offer["i18n"]["displayName"].is_null())
						{
							vector<string> genres(sregex_token_iterator(genreFacetKey.begin(), genreFacetKey.end(), comma, -1),
								sregex_token_iterator());

							for (auto genre : genres)
							{
									catalogue_game_mapping_offer_id[genre][franchiseFacetKey][gameNameFacetKey].insert(offer_id);
									if (!offer_path.empty())
										catalogue_game_mapping_offer_path[genre][franchiseFacetKey][gameNameFacetKey].insert(offer_path);
							}
						}
					}

					ConvertBWCurrency(two_letter_country_code, bioware_virtual_currency_offer_id, offer_ids_with_bw);
				}
			}
		}
	}

	void OriginQueries::ConvertBWCurrency(const string &two_letter_country_code, const string &bioware_virtual_currency_offer_id, const vector<string> &offer_ids_with_bw)
	{
		if (!offer_ids_with_bw.empty() && !bioware_virtual_currency_offer_id.empty())
		{
			double bw_in_local_currency;
			string currency = two_letter_country_map_to_currency_name[two_letter_country_code].ToStdString();
			shared_ptr<GameDetailData> bioware_virtual_currency_data = offer_id_map_to_game_detail[bioware_virtual_currency_offer_id];
			if (bioware_virtual_currency_data->price_normal.find(two_letter_country_code) != bioware_virtual_currency_data->price_normal.end())
				bw_in_local_currency = bioware_virtual_currency_data->price_normal[two_letter_country_code] / 1600;
			else
				bw_in_local_currency = bioware_virtual_currency_data->price_normal[currency] / 1600;

			for (auto offer_id : offer_ids_with_bw)
			{
				shared_ptr<GameDetailData> offer_detail_data = offer_id_map_to_game_detail[offer_id];
				if (!offer_detail_data->price_normal.empty())
				{
					double bw_price = offer_detail_data->price_normal["_BW"];
					double actual_price = bw_in_local_currency * bw_price;
					offer_detail_data->price_normal.erase("_BW");
					offer_detail_data->price_normal[currency] = actual_price;
				}
			}
		}
	}

	void OriginQueries::UpdateDiscountInfo(const string &two_letter_country_code, const string &offer_id, const string &response_string)
	{
		if (!response_string.empty())
		{
			if (response_string.find("<HTML>") != string::npos && response_string.find("</HTML>") != string::npos) return;
			string currency = two_letter_country_map_to_currency_name[two_letter_country_code].ToStdString();
			if (response_string.find(R"(xml version="1.0")") != string::npos)
			{
				unique_ptr<xml_document<>> xml_doc = make_unique<xml_document<>>();
				string discount_string = response_string;
				xml_doc->parse<0>(discount_string.data());

				xml_node<> *offer_node = xml_doc->first_node("offerRatingResults")->first_node("offer");
				xml_node<> *offer_id_node = offer_node->first_node("offerId");
				if (offer_id_node == nullptr) return;
				if (offer_id_node->value() == offer_id)
				{
					xml_node<> *rating_node = offer_node->first_node("rating");
					if (rating_node == nullptr) return;
					if (xml_attribute<> *attr = rating_node->first_attribute(); attr != nullptr)
					{
						string discount_currency = attr->value();
						xml_node<> *finalTotalAmount_node = rating_node->first_node("finalTotalAmount");
						offer_id_map_to_game_detail[offer_id]->price_discounted[discount_currency] = atof(finalTotalAmount_node->value());
					}
				}
			}
			else
			{
				try
				{
					json discount_json = json::parse(response_string);
					if (json discount_offer_array = discount_json["offer"]; discount_offer_array.is_array())
					{
						for (json &discount_offer : discount_offer_array)
						{
							if (discount_offer["offerId"] == offer_id)
							{
								string discount_currency = discount_offer["rating"][0]["currency"].is_null() ?
									currency : discount_offer["rating"][0]["currency"].get<string>();
								double discount_price = discount_offer["rating"][0]["finalTotalAmount"].is_null() ?
									0 : discount_offer["rating"][0]["finalTotalAmount"].get<double>();
								offer_id_map_to_game_detail[offer_id]->price_discounted[discount_currency] = discount_price;
							}
						}
					}
				}
				catch (json::exception &ex)
				{
					wxMessageBox(ex.what(), __FUNCTION__);
				}
			}
		}
	}

	void OriginQueries::RetrieveTranslation(const string &three_letter_country_code, const string &origin_display_language_code)
	{
		wxString translations_url = origin_data_api + origin_shell_translation;
		translations_url.Replace("{num}", GenerateNumberInString(1, 4));
		translations_url.Replace("{countrylocale}", to_hyphen_copy(to_lower_copy(origin_display_language_code)) + "." + to_lower_copy(three_letter_country_code));
		
		string reformat_lng_code = to_underline_copy(to_lower_copy(origin_display_language_code));
		reformat_lng_code[3] = toupper(reformat_lng_code[3]);
		reformat_lng_code[4] = toupper(reformat_lng_code[4]);

		if (keyword_translations.find(reformat_lng_code) != keyword_translations.end()) return;

		wxString file_content = GetFileContentFromURL(translations_url);
		json origin_keyword;
		try
		{
			origin_keyword = json::parse(file_content.utf8_string());
		}
		catch (json::exception &ex)
		{
			wxMessageBox(ex.what(), __FUNCTION__);
		}

		for (auto each_node : origin_keyword)
		{
			for (auto &[key, value] : each_node.items())
			{
				keyword_translations[reformat_lng_code][wxString::FromUTF8(key).ToStdString()] = wxString::FromUTF8(value).ToStdString();
			}
		}
	}

	void OriginQueries::InitialTimeSettings()
	{
		time_t local_raw_time = time(nullptr);
		tm temp_tm = *gmtime(&local_raw_time);
		temp_tm.tm_isdst = -1;
		time_t gmt_time = mktime(&temp_tm);
		time_zone_hours = difftime(local_raw_time, gmt_time) / 60 / 60;
	}

	void OriginQueries::DownloadGameList(string two_letter_country_code, string origin_display_language_code, function<void(string newmsg)> update_progress)
	{
		if (downloaded_language_code.find(origin_display_language_code) != downloaded_language_code.end()) return;

		string three_letter_country_code = two_letter_country_map_to_three_letter_country[two_letter_country_code].ToStdString();
		auto bind_func_translation = bind(&OriginQueries::RetrieveTranslation, this, placeholders::_1, placeholders::_2);
		auto get_translation_thread = async(launch::async, bind_func_translation, reference_wrapper(three_letter_country_code), reference_wrapper(origin_display_language_code));

		wxString wx_language_code_underline = to_underline_copy(origin_display_language_code);
		wxString wx_supercat_lng_code = wx_language_code_underline;
		wx_supercat_lng_code.MakeLower();
		wx_supercat_lng_code[3] = toupper(wx_supercat_lng_code[3]);
		wx_supercat_lng_code[4] = toupper(wx_supercat_lng_code[4]);

		map<string, future<string>> game_info_handles;
		map<string, future<vector<string>>> onslae_handles;
		auto bind_func_supercat = bind(&OriginQueries::RetreiveGameInfo, this, placeholders::_1, placeholders::_2);
		auto bind_func_onsale = bind(&OriginQueries::RetrieveOnSaleList, this, placeholders::_1, placeholders::_2);
		for (auto &[country_code, currency_name] : two_letter_country_map_to_currency_name)
		{
			game_info_handles[country_code.ToStdString()] = async(launch::async, bind_func_supercat, country_code, wx_supercat_lng_code);			
			onslae_handles[country_code.ToStdString()] = async(launch::async, bind_func_onsale, country_code, wx_supercat_lng_code);
		}

		update_progress("get_full_list");

		map<string, string> game_info_mapping;
		for (auto &[country_code, handle] : game_info_handles)
		{
			game_info_mapping[country_code] = handle.get();
		}

		update_progress("get_onsale_list");

		map<string, vector<string>> onslae_mapping;
		for (auto &[country_code, handle] : onslae_handles)
		{
			onslae_mapping[country_code] = handle.get();
		}

		map<string, vector<string>> onslae_offerid_mapping;
		map<string, future<map<string, string>>> discount_handles;
		UnpackSupercatData(game_info_mapping, onslae_mapping, wx_supercat_lng_code.ToStdString(), onslae_offerid_mapping);
		if (!onslae_offerid_mapping.empty())
		{
			string language_code_underline_lower_string = wx_language_code_underline.ToStdString();
			to_lower(language_code_underline_lower_string);
			auto bind_func_discount = bind(&OriginQueries::QueryDiscounts, this, placeholders::_1);
			for (auto &[country_code, offer_ids] : onslae_offerid_mapping)
			{
				string currency = two_letter_country_map_to_currency_name[country_code].ToStdString();
				map<string, string> onslae_urls_mapping;
				for (auto &offer_id : offer_ids)
				{
					//discount_handles[country_code][offer_id] = async(launch::async, bind_func_discount, country_code, language_code_underline_lower_string, currency, offer_id);
					onslae_urls_mapping[offer_id] = GetDiscountURL(country_code, language_code_underline_lower_string, currency, offer_id);
				}
				discount_handles[country_code] = async(launch::async, bind_func_discount, onslae_urls_mapping);
			}
		}

		update_progress("get_discount_list");

		for (auto &[country_code, handle] : discount_handles)
		{
			for (auto &[offer_id, result] : handle.get())
			{
				UpdateDiscountInfo(country_code, offer_id, result);
			}
		}

		get_translation_thread.wait();
		downloaded_language_code.insert(origin_display_language_code);
	}

	vector<string> OriginQueries::RetrieveOnSaleList(wxString two_letter_country_code, wxString language_code_underline)
	{
		vector<string> discount_list;
		string three_letter_country = two_letter_country_map_to_three_letter_country[two_letter_country_code].ToStdString();
		wxString onsale_full_url = origin_base_api + origin_search_onsale_json;
		onsale_full_url.Replace("{num}", GenerateNumberInString(1, 4));
		onsale_full_url.Replace("{threeLetterCountry}", to_lower_copy(three_letter_country));
		onsale_full_url.Replace("{locale}", to_lower_copy(language_code_underline.ToStdString()));

		for (int count = 0, total = 1; count < total;)
		{
			wxString onsale_full_url_start = onsale_full_url;
			onsale_full_url_start.Replace("{start_pos}", to_string(count));
			wxString file_content = GetFileContentFromURL(onsale_full_url_start);
			if (file_content.empty()) continue;
			try
			{
				json discount_info = json::parse(file_content.utf8_string());
				total = discount_info["games"]["numFound"].get<int>();

				if (int num_visible = discount_info["games"]["numVisible"].get<int>(); num_visible > 0)
				{
					for (auto &[key, value] : discount_info["games"]["game"].items())
					{
						discount_list.push_back(value["path"]);
					}

					count += num_visible;
				}
				else break;
			}
			catch (json::exception &ex)
			{
				wxMessageBox(ex.what(), __FUNCTION__);
				break;
			}
		}

		return discount_list;
	}
}
