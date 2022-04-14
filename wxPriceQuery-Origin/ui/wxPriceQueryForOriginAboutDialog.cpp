#include "wxPriceQueryForOriginAboutDialog.h"

wxPriceQueryForOriginAboutDialog::wxPriceQueryForOriginAboutDialog( wxWindow* parent )
:
AboutDialog( parent )
{
	wxLocale wl;
	wl.Init();
	current_language = languages::EnumMappingList()[wl.GetLanguage()];

	m_hyperlink_project->SetLabelText("wxPriceQuery-Origin");
	m_hyperlink_project->SetURL("https://github.com/cnbatch/wxPriceQuery-Origin");
	m_hyperlink_api1->SetLabelText("Euro foreign exchange reference rates");
	m_hyperlink_api1->SetURL("https://www.ecb.europa.eu/stats/policy_and_exchange_rates/euro_reference_exchange_rates/html/index.en.html");
	m_hyperlink_api2->SetLabelText("ExchangeRate-API");
	m_hyperlink_api2->SetURL("https://www.exchangerate-api.com/");
}

void wxPriceQueryForOriginAboutDialog::SetLanguage(languages::Language lng)
{
	current_language = lng;

	this->SetTitle(languages::TranslateStaticText(current_language, "about"));
	m_staticText_version->SetLabelText(languages::TranslateStaticText(current_language, "app_title") + " (v0.5)");
	m_staticText_name->SetLabelText(languages::TranslateStaticText(current_language, "this_project"));
	m_staticText_apis->SetLabelText(languages::TranslateStaticText(current_language, "api_exchange_rate"));

	this->Fit();
	this->Refresh();
}
