#include "CurrencyAPI.hpp"
#include <curl/curl.h>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "../../string_utilities.hpp"

namespace query_tools
{
	using namespace std;
	using nlohmann::json;
	using namespace string_utitlies;

	typedef size_t(*CURL_CALLBACK_FUNCTION_PTR)(char*, size_t, size_t, void*);

	/*map<string, double> CurrencyAPI::exchange_rates;
	string CurrencyAPI::base_currency;*/

	CurrencyAPI::CurrencyAPI()
	{
		UpdateExchangeRate();
		UpdateExchangeRateTWD();
	}

	vector<string> CurrencyAPI::GetAvailableCurrency()
	{
		vector<string> currency_list;
		if (exchange_rates.empty())
		{
			UpdateExchangeRate();
			UpdateExchangeRateTWD();
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
		auto deleter = [](CURL *handle) { if (handle) curl_easy_cleanup(handle); };
		unique_ptr<CURL, decltype(deleter)> curl_handle(curl_easy_init(), deleter);
		vector<char> received_data;
		curl_easy_setopt(curl_handle.get(), CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &received_data);
		curl_easy_setopt(curl_handle.get(), CURLOPT_URL, "https://api.exchangeratesapi.io/latest");
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
		base_currency = to_upper_copy(currency_json["base"]);
		for (auto &[key, value] : currency_json["rates"].items())
		{
			exchange_rates[to_upper_copy(key)] = value.get<double>();
		}
		exchange_rates[base_currency] = 1;
	}

	void CurrencyAPI::UpdateExchangeRateTWD()
	{
		auto deleter = [](CURL *handle) { if (handle) curl_easy_cleanup(handle); };
		unique_ptr<CURL, decltype(deleter)> curl_handle(curl_easy_init(), deleter);
		vector<char> received_data;
		curl_easy_setopt(curl_handle.get(), CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &received_data);
		curl_easy_setopt(curl_handle.get(), CURLOPT_URL, "https://www.freeforexapi.com/api/live?pairs=USDEUR,USDTWD");
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
			exchange_rates["TWD"] = currency_json["rates"]["USDTWD"]["rate"].get<double>() / currency_json["rates"]["USDEUR"]["rate"].get<double>();
		}
	}
}
