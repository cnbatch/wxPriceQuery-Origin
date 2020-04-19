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
		void UpdateExchangeRateTWD();

	public:
		CurrencyAPI();
		std::vector<std::string> GetAvailableCurrency();
		double ConvertCurrency(const std::string &from_currency_code, const std::string &to_currency_code, double amount);
	};
}
