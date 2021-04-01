#include "CurrencyAPI.hpp"
#include <curl/curl.h>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#ifdef _WIN32
#include <rapidxml/rapidxml.hpp>
#else
#include <rapidxml.h>
#endif
#include "../../string_utilities.hpp"

namespace query_tools
{
	using namespace std;
	using nlohmann::json;
	using namespace rapidxml;
	using namespace string_utitlies;

	typedef size_t(*CURL_CALLBACK_FUNCTION_PTR)(char*, size_t, size_t, void*);

	/*map<string, double> CurrencyAPI::exchange_rates;
	string CurrencyAPI::base_currency;*/

	CurrencyAPI::CurrencyAPI()
	{
		UpdateExchangeRate();
		UpdateExchangeRateOthers();
	}

	vector<string> CurrencyAPI::GetAvailableCurrency()
	{
		vector<string> currency_list;
		if (exchange_rates.empty())
		{
			UpdateExchangeRate();
			UpdateExchangeRateOthers();
		}

		transform(begin(exchange_rates), end(exchange_rates), back_inserter(currency_list), [](auto const &each_pair) { return each_pair.first; });
		return currency_list;
	}

	double CurrencyAPI::ConvertCurrency(const string &from_currency_code, const string &to_currency_code, double amount)
	{
		if (to_upper_copy(from_currency_code) == to_upper_copy(to_currency_code))
		{
			return amount;
		}

		return amount / exchange_rates.at(from_currency_code) * exchange_rates.at(to_currency_code);
	}

	void CurrencyAPI::UpdateExchangeRate()
	{
		base_currency = "EUR";
		exchange_rates[base_currency] = 1;

		auto deleter = [](CURL *handle) { if (handle) curl_easy_cleanup(handle); };
		unique_ptr<CURL, decltype(deleter)> curl_handle(curl_easy_init(), deleter);
		vector<char> received_data;
		curl_easy_setopt(curl_handle.get(), CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &received_data);
		curl_easy_setopt(curl_handle.get(), CURLOPT_URL, "https://www.ecb.europa.eu/stats/eurofxref/eurofxref-daily.xml");
		curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEFUNCTION,
			static_cast<CURL_CALLBACK_FUNCTION_PTR>([](char *ptr, size_t size, size_t nmemb, void *userdata) -> size_t
				{
					vector<char> &received_data = *reinterpret_cast<vector<char> *>(userdata);
					received_data.insert(received_data.end(), ptr, ptr + nmemb);
					return size * nmemb;
				}));

		if (CURLcode curl_result_code = curl_easy_perform(curl_handle.get()); curl_result_code != CURLE_OK)
		{
			return;
		}

		string currency_xml_string(received_data.begin(), received_data.end());

		unique_ptr<xml_document<>> xml_doc = make_unique<xml_document<>>();
		xml_doc->parse<0>(currency_xml_string.data());

		xml_node<> *cube_node = xml_doc->first_node()->first_node("Cube")->first_node("Cube");
		if (cube_node == nullptr) return;
		for (xml_node<> *currency_node = cube_node->first_node(); currency_node; currency_node = currency_node->next_sibling())
		{
			string currenct = currency_node->first_attribute("currency")->value();
			double rate = atof(currency_node->first_attribute("rate")->value());
			exchange_rates[currenct] = rate;
		}
	}

	void CurrencyAPI::UpdateExchangeRateOthers()
	{
		std::vector<std::string> url_pairs = other_currency_names;
		std::for_each(url_pairs.begin(), url_pairs.end(), [](auto &str) { str.insert(0, "USD"); });
		std::string url_part;
		for (const auto &str : url_pairs)
			url_part = url_part + "," + str;
		std::string url = "https://www.freeforexapi.com/api/live?pairs=USDEUR" + url_part;

		auto deleter = [](CURL *handle) { if (handle) curl_easy_cleanup(handle); };
		unique_ptr<CURL, decltype(deleter)> curl_handle(curl_easy_init(), deleter);
		vector<char> received_data;
		curl_easy_setopt(curl_handle.get(), CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &received_data);
		curl_easy_setopt(curl_handle.get(), CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEFUNCTION,
			static_cast<CURL_CALLBACK_FUNCTION_PTR>([](char *ptr, size_t size, size_t nmemb, void *userdata) -> size_t
				{
					vector<char> &received_data = *reinterpret_cast<vector<char> *>(userdata);
					received_data.insert(received_data.end(), ptr, ptr + nmemb);
					return size * nmemb;
				}));

		if (CURLcode curl_result_code = curl_easy_perform(curl_handle.get()); curl_result_code != CURLE_OK)
		{
			return;
		}

		string currency_json_string(received_data.begin(), received_data.end());
		json currency_json = json::parse(currency_json_string);
		if (currency_json.find("message") == currency_json.end())
		{
			for (const auto &currency_name : other_currency_names)
			exchange_rates[currency_name] = currency_json["rates"]["USD" + currency_name]["rate"].get<double>() / currency_json["rates"]["USDEUR"]["rate"].get<double>();
		}
	}
}
