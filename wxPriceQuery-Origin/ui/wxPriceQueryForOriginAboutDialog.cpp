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
	m_hyperlink_api1->SetLabelText("exchangeratesapi.io");
	m_hyperlink_api1->SetURL("https://www.exchangeratesapi.io/");
	m_hyperlink_api2->SetLabelText("Free Forex API");
	m_hyperlink_api2->SetURL("https://www.freeforexapi.com/");
}

void wxPriceQueryForOriginAboutDialog::SetLanguage(languages::Language lng)
{
	current_language = lng;

	this->SetTitle(languages::TranslateStaticText(current_language, "about"));
	m_staticText_version->SetLabelText(languages::TranslateStaticText(current_language, "app_title") + " (v0.12)");
	m_staticText_name->SetLabelText(languages::TranslateStaticText(current_language, "this_project"));
	m_staticText_apis->SetLabelText(languages::TranslateStaticText(current_language, "api_exchange_rate"));

	this->Fit();
	this->Refresh();
}
