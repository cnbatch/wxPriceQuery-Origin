#pragma once
#include <string>
#include <map>
#include <vector>
#include <shared_mutex>
#include <wx/event.h>

namespace currency_tools
{
	constexpr int retry_times = 5;

	class CurrencyAPI
	{
	private:
		wxEvtHandler *window_handler;
		mutable std::shared_mutex current_class_mutex;
		std::map<std::string, double> exchange_rates;
		std::map<int, wxString> web_id_map_string;
		std::string base_currency;
		
		wxString GetFileContentFromURL(wxString url, int retry = retry_times);
		void UpdateExchangeRates();
		std::vector<std::string> other_currency_names = { "TWD", "ARS", "CLP", "COP", "EGP", "VND" };

	public:
		CurrencyAPI();
		CurrencyAPI(wxEvtHandler *current_window_handler);
		void SetWindowHandler(wxEvtHandler *current_window_handler);
		std::vector<std::string> GetAvailableCurrency();
		double ConvertCurrency(const std::string &from_currency_code, const std::string &to_currency_code, double amount);
	};
}
