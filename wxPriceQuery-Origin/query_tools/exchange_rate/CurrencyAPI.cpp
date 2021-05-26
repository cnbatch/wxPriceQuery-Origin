#include "CurrencyAPI.hpp"
#include <wx/webrequest.h>
#include <memory>
#include <vector>
#include <future>
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

	CurrencyAPI::CurrencyAPI() : window_handler(nullptr)
	{
	}

	CurrencyAPI::CurrencyAPI(wxEvtHandler *current_window_handler) : window_handler(current_window_handler)
	{
		async(launch::async, bind(&CurrencyAPI::UpdateExchangeRates, this)).wait();
	}

	void CurrencyAPI::SetWindowHandler(wxEvtHandler *current_window_handler)
	{
		window_handler = current_window_handler;
		async(launch::async, bind(&CurrencyAPI::UpdateExchangeRates, this)).wait();
	}

	vector<string> CurrencyAPI::GetAvailableCurrency()
	{
		vector<string> currency_list;
		if (exchange_rates.empty())
		{
			async(launch::async, bind(&CurrencyAPI::UpdateExchangeRates, this)).wait();
		}

		transform(begin(exchange_rates), end(exchange_rates), back_inserter(currency_list),
			[](auto const &each_pair) { return each_pair.first; });
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

	wxString CurrencyAPI::GetFileContentFromURL(wxString url)
	{
		wxString trim_url = url;
		trim_url.Trim();

		wxWebRequest web_request;
		wxWebSession web_session = wxWebSession::New();
		if (web_session.IsOpened())
			web_request = web_session.CreateRequest(window_handler, trim_url);
		else
			web_request = wxWebSession::GetDefault().CreateRequest(window_handler, trim_url);

		if (!web_request.IsOk())
			return wxString();

		web_request.SetHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0");

		web_request.Start();
		while (web_request.GetState() == wxWebRequest::State_Active)
		{
			this_thread::sleep_for(100ms);
		}

		return web_request.GetResponse().AsString();
	}

	void CurrencyAPI::UpdateExchangeRates()
	{
		base_currency = "EUR";
		exchange_rates[base_currency] = 1;
		string ecb_url = "https://www.ecb.europa.eu/stats/eurofxref/eurofxref-daily.xml";

		string exchangerate_api_url = "https://open.er-api.com/v6/latest/EUR";


		auto wait_0 = async(launch::async, bind(&CurrencyAPI::GetFileContentFromURL, this, placeholders::_1), ecb_url);
		auto wait_1 = async(launch::async, bind(&CurrencyAPI::GetFileContentFromURL, this, placeholders::_1), exchangerate_api_url);
		string currency_xml_string = wait_0.get().ToStdString();
		string currency_json_string = wait_1.get().ToStdString();

		if (!currency_xml_string.empty())
		{
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

		if (!currency_json_string.empty())
		{
			if (currency_xml_string.empty())
			{
				json currency_json = json::parse(currency_json_string);
				if (currency_json.find("rates") != currency_json.end())
				{
					for (const auto &currency_pair : currency_json["rates"].get<json::object_t>())
						exchange_rates[currency_pair.first] = currency_pair.second.get<double>();
				}
			}
			else
			{
				json currency_json = json::parse(currency_json_string);
				if (currency_json.find("rates") != currency_json.end())
				{
					for (const auto &currency_name : other_currency_names)
						exchange_rates[currency_name] = currency_json["rates"][currency_name].get<double>();
				}
			}
		}
	}
}
