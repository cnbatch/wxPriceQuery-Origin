#include "wxPriceQueryForOriginSettingDialog.h"

wxPriceQueryForOriginSettingDialog::wxPriceQueryForOriginSettingDialog(wxWindow* parent)
	:
	SettingDialog(parent)
{
	InitialiseApplication();
}

void wxPriceQueryForOriginSettingDialog::OnChoiceInterfaceLanguage(wxCommandEvent& event)
{
	if (auto selected_item = m_choice_interface_lng->GetSelection(); selected_item != wxNOT_FOUND)
	{
		if (interface_selection_list[selected_item] != current_language)
		{
			current_language = interface_selection_list[selected_item];

			sbSizer_Setting_Interface->GetStaticBox()->SetLabel(languages::TranslateStaticText(current_language, "interface_settings"));
			m_staticText_interface_lng->SetLabel(languages::TranslateStaticText(current_language, "interface_language"));
			m_staticText_my_currency->SetLabel(languages::TranslateStaticText(current_language, "my_currency"));
			sbSizer_Setting_Origin->GetStaticBox()->SetLabel(languages::TranslateStaticText(current_language, "origin_parameters"));
			m_staticText_game_lng->SetLabel(languages::TranslateStaticText(current_language, "lng_game_list"));
			m_staticText_Region->SetLabel(languages::TranslateStaticText(current_language, "region"));
			m_sdbSizerButtons->GetAffirmativeButton()->SetLabel(languages::TranslateStaticText(current_language, "ok_btn"));
			m_sdbSizerButtons->GetCancelButton()->SetLabel(languages::TranslateStaticText(current_language, "cancel_btn"));
	
			auto selected_currency = m_choice_currency->GetSelection();
			auto selected_region = m_choice_Region->GetSelection();

			m_choice_currency->Clear();
			m_choice_Region->Clear();

			for (auto &currency_name : currency_api_ptr->GetAvailableCurrency())
			{
				m_choice_currency->AppendString(languages::TranslateCurrencyName(current_language, currency_name));
			}
			
			auto two_letter_country_map_to_currency_name = queries_ptr->GetCurrencyList();
			for (const auto &[key, value] : two_letter_country_map_to_currency_name)
			{
				m_choice_Region->AppendString(languages::TranslateCountryCode(current_language, key));
			}

			if (selected_currency != wxNOT_FOUND)
				m_choice_currency->SetSelection(selected_currency);

			if (selected_region != wxNOT_FOUND)
				m_choice_Region->SetSelection(selected_region);
		}
	}
}

void wxPriceQueryForOriginSettingDialog::OnClockCancel(wxCommandEvent& event)
{
	this->EndModal(wxID_CANCEL);
}

void wxPriceQueryForOriginSettingDialog::OnClickOK(wxCommandEvent& event)
{
	wxString message, caption;
	
	if (m_choice_currency->GetSelection() == wxNOT_FOUND)
	{
		caption = languages::TranslateStaticText(current_language, "selection_missing");
		message = languages::TranslateStaticText(current_language, "select_currency");
		wxMessageBox(message, caption);
		return;
	}

	if (m_choice_game_lng->GetSelection() == wxNOT_FOUND)
	{
		caption = languages::TranslateStaticText(current_language, "selection_missing");
		message = languages::TranslateStaticText(current_language, "select_language");
		wxMessageBox(message, caption);
		return;
	}

	if (m_choice_Region->GetSelection() == wxNOT_FOUND)
	{
		caption = languages::TranslateStaticText(current_language, "selection_missing");
		message = languages::TranslateStaticText(current_language, "select_region");
		wxMessageBox(message, caption);
		return;
	}

	this->EndModal(wxID_OK);
}


void wxPriceQueryForOriginSettingDialog::GetClientSettings(languages::Language &interface_lng, wxString &selected_currency, wxString &origin_lng, wxString &origin_country)
{
	interface_lng = current_language;
	selected_currency = currency_selection_list[m_choice_currency->GetSelection()];
	origin_lng = game_language_selection_list[m_choice_game_lng->GetSelection()];
	origin_country = country_selection_list[m_choice_Region->GetSelection()];
}

void wxPriceQueryForOriginSettingDialog::InitialiseApplication()
{
	current_language = languages::EnumMappingList()[wxLocale::GetSystemLanguage()];
	this->SetLabel(languages::TranslateStaticText(current_language, "settings"));
	for (auto &language_list : languages::InterfaceLanguagesList())
	{
		interface_selection_list.push_back(language_list.first);
		m_choice_interface_lng->AppendString(language_list.second);
	}

	sbSizer_Setting_Interface->GetStaticBox()->SetLabel(languages::TranslateStaticText(current_language, "interface_settings"));
	m_staticText_interface_lng->SetLabel(languages::TranslateStaticText(current_language, "interface_language"));
	m_staticText_my_currency->SetLabel(languages::TranslateStaticText(current_language, "my_currency"));
	sbSizer_Setting_Origin->GetStaticBox()->SetLabel(languages::TranslateStaticText(current_language, "origin_parameters"));
	m_staticText_game_lng->SetLabel(languages::TranslateStaticText(current_language, "lng_game_list"));
	m_staticText_Region->SetLabel(languages::TranslateStaticText(current_language, "region"));
	m_sdbSizerButtons->GetAffirmativeButton()->SetLabel(languages::TranslateStaticText(current_language, "ok_btn"));
	m_sdbSizerButtons->GetCancelButton()->SetLabel(languages::TranslateStaticText(current_language, "cancel_btn"));

	m_choice_interface_lng->SetStringSelection(languages::InterfaceLanguagesList()[current_language]);
}

bool wxPriceQueryForOriginSettingDialog::InitialiseConnections(std::function<void(std::string newmsg)> update_progress)
{
	auto get_currency = std::bind(&query_tools::CurrencyAPI::GetAvailableCurrency, currency_api_ptr.get());
	auto currency_names = std::async(std::launch::async, get_currency);

	for (auto &currency_name : currency_names.get())
	{
		currency_selection_list.push_back(currency_name);
		m_choice_currency->AppendString(languages::TranslateCurrencyName(current_language, currency_name));
	}

	auto [result, error_str] = queries_ptr->ConnectOriginServer();
	if (result)
	{
		auto language_code_hyphen_map_to_display_language = queries_ptr->GetDisplayLanguageList(update_progress);
		for (const auto &[key, value] : language_code_hyphen_map_to_display_language)
		{
			game_language_selection_list.push_back(key);
			m_choice_game_lng->AppendString(value);
		}

		auto two_letter_country_map_to_currency_name = queries_ptr->GetCurrencyList();
		for (const auto &[key, value] : two_letter_country_map_to_currency_name)
		{
			country_selection_list.push_back(key);
			m_choice_Region->AppendString(languages::TranslateCountryCode(current_language, key));
		}

		return true;
	}
	else return false;
}