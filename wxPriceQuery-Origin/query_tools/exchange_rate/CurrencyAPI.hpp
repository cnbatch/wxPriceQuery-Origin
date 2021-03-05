#pragma once
#include <string>
#include <map>
#include <vector>

namespace query_tools
{
	class CurrencyAPI
	{
	private:
		std::map<std::string, double> exchange_rates;
		std::string base_currency;
		void UpdateExchangeRate();
		void UpdateExchangeRateOthers();
		std::vector<std::string> other_currency_names = { "TWD", "ARS", "CLP", "COP", "EGP", "VND" };

	public:
		CurrencyAPI();
		std::vector<std::string> GetAvailableCurrency();
		double ConvertCurrency(const std::string &from_currency_code, const std::string &to_currency_code, double amount);
	};
}
