#include "wxPriceQueryForOriginUIFrame.h"
#include <wx/msgdlg.h>
#include <sstream>
#include <chrono>
#include "../string_utilities.hpp"

wxPriceQueryForOriginUIFrame::wxPriceQueryForOriginUIFrame(wxWindow* parent)
	:
	UIFrame(parent)
{
	wxLocale wl;
	wl.Init();
	current_language = languages::EnumMappingList()[wl.GetLanguage()];
	setting_dialog.SetIcon(this->GetIcon());
	m_timer_loading.StartOnce(100);
}

void wxPriceQueryForOriginUIFrame::OnIdle(wxIdleEvent& event)
{
	if (loading_completed)
	{
		loading_completed = false;
		if (setting_dialog.ShowModal() != wxID_OK)
		{
			this->Destroy();
			return;
		}

		setting_dialog.GetClientSettings(current_language, selected_currency, origin_lng, origin_two_letter_country);
		origin_lng.MakeLower();
		origin_lng[3] = toupper(origin_lng[3]);
		origin_lng[4] = toupper(origin_lng[4]);

		SetupLabelText();
		this->Show();
		this->SetFocus();
		run_idle_event_process = true;
	}

	if (run_idle_event_process)
	{
		run_idle_event_process = false;
		progress_dialog = new wxProgressDialog(
			languages::TranslateStaticText(current_language, "wait"),
			languages::TranslateStaticText(current_language, "download"),
			100, this, wxPD_SMOOTH | wxPD_APP_MODAL);
		progress_dialog->Pulse();
		progress_dialog->Refresh();
		progress_dialog->Update();
		std::thread get_list_thread([&, this]()
			{
				queries_ptr->DownloadGameList(origin_two_letter_country.ToStdString(), string_utitlies::to_underline_copy(origin_lng.ToStdString()), [&](std::string newmsg)
					{
						static int count = 0;
						progress_dialog->Pulse();
					});
				WriteCatalogueToTreeList(string_utitlies::to_underline_copy(origin_lng.ToStdString()));
				progress_dialog->Destroy();
				progress_dialog = nullptr;
			});
		get_list_thread.detach();
	}

	event.Skip();
}

void wxPriceQueryForOriginUIFrame::OnTextInputSearch(wxCommandEvent& event)
{
	if (m_timer_search.IsRunning()) return;
	m_timer_search.StartOnce(1000);
}

void wxPriceQueryForOriginUIFrame::OnTreelistGameListSelectionChanged(wxTreeListEvent& event)
{
	m_staticText_languages->SetLabel("");
	m_staticText_currency_order->SetLabel("");
	
	if (!m_searchCtrl_Left->GetValue().IsEmpty()) return;
	m_treeListCtrl_ItemPrice->DeleteAllItems();

	wxTreeListItem selected_item = m_treeListCtrl_Left->GetSelection();
	if (nullptr == selected_item)
	{
		return;
	}
	if (m_treeListCtrl_Left->GetFirstItem() == selected_item)
	{
		auto offer_id_map_to_detail_info = queries_ptr->GetOfferIDMapping();
		auto offer_path_map_to_offer_id = queries_ptr->GetOfferIdFromOfferPathMap();

		for (auto &[offer_path, offer_ids] : offer_path_map_to_offer_id)
		{
			if (offer_ids.size() == 0) continue;
			WriteDetailDataToTreeList(origin_two_letter_country.ToStdString(), string_utitlies::to_underline_copy(origin_lng.ToStdString()), offer_id_map_to_detail_info, offer_ids);
		}
	}
	else
	{
		std::unordered_set<std::string> offer_pathes;
		auto catalogue_offer_path_mapping = queries_ptr->GetCatalogueMappingOfferPath();
		if (m_treeListCtrl_Left->GetItemText(selected_item, 1) == "genreFacetKey")
		{
			auto genreFacetKey = m_treeListCtrl_Left->GetItemText(selected_item, 2).ToStdString();
			for (auto franchise_layer : catalogue_offer_path_mapping[genreFacetKey])
			{
				for (auto gamename_layer : franchise_layer.second)
				{
					offer_pathes.insert(gamename_layer.second.begin(), gamename_layer.second.end());
				}
			}

		}
		else if (m_treeListCtrl_Left->GetItemText(selected_item, 1) == "franchiseFacetKey")
		{
			auto franchiseFacetKey = m_treeListCtrl_Left->GetItemText(selected_item, 2).ToStdString();
			auto genreFace_level = m_treeListCtrl_Left->GetItemParent(selected_item);
			auto genreFacetKey = m_treeListCtrl_Left->GetItemText(genreFace_level, 2).ToStdString();
			for (auto gamename_layer : catalogue_offer_path_mapping[genreFacetKey][franchiseFacetKey])
			{
				offer_pathes.insert(gamename_layer.second.begin(), gamename_layer.second.end());
			}
		}
		else if (m_treeListCtrl_Left->GetItemText(selected_item, 1) == "gameNameFacetKey")
		{
			auto gameNameFacetKey = m_treeListCtrl_Left->GetItemText(selected_item, 2).ToStdString();
			auto franchise_level = m_treeListCtrl_Left->GetItemParent(selected_item);
			auto franchiseFacetKey = m_treeListCtrl_Left->GetItemText(franchise_level, 2).ToStdString();
			auto genreFace_level = m_treeListCtrl_Left->GetItemParent(franchise_level);
			auto genreFacetKey = m_treeListCtrl_Left->GetItemText(genreFace_level, 2).ToStdString();
			auto &pathes = catalogue_offer_path_mapping[genreFacetKey][franchiseFacetKey][gameNameFacetKey];
			offer_pathes.insert(pathes.begin(), pathes.end());
		}

		auto offer_id_map_to_detail_info = queries_ptr->GetOfferIDMapping();
		auto offer_path_map_to_offer_ids = queries_ptr->GetOfferIdFromOfferPathMap();

		for (auto offer_path : offer_pathes)
		{
			auto offer_ids = offer_path_map_to_offer_ids[offer_path];
			WriteDetailDataToTreeList(origin_two_letter_country.ToStdString(), string_utitlies::to_underline_copy(origin_lng.ToStdString()), offer_id_map_to_detail_info, offer_ids);
		}
	}
	//m_treeListCtrl_ItemPrice->SetSortColumn(4);
}

void wxPriceQueryForOriginUIFrame::OnTreelistGamePriceSelectionChanged(wxTreeListEvent& event)
{
	wxTreeListItem selected_item = m_treeListCtrl_ItemPrice->GetSelection();
	if (nullptr == selected_item)
	{
		m_staticText_languages->SetLabel("");
		m_staticText_currency_order->SetLabel("");
		return;
	}

	std::vector<std::string> supported_languages_list, other_region_languages_list;
	std::string offer_path = m_treeListCtrl_ItemPrice->GetItemText(selected_item, 1).ToStdString();
	auto offer_id_map_to_detail_info = queries_ptr->GetOfferIDMapping();
	auto offer_path_map_to_offer_ids = queries_ptr->GetOfferIdFromOfferPathMap();

	for (auto offer_id : offer_path_map_to_offer_ids[offer_path])
	{
		if (auto &country_names = offer_id_map_to_detail_info[offer_id]->avaliable_country_two_letter; country_names.find(origin_two_letter_country) != country_names.end())
		{
			for (auto language : offer_id_map_to_detail_info[offer_id]->supported_languages)
			{
				if (!language.IsEmpty())
					supported_languages_list.emplace_back(language);
			}
			std::unordered_set<std::string> remove_duplicate_set(supported_languages_list.begin(), supported_languages_list.end());
			supported_languages_list.clear();
			supported_languages_list.assign(remove_duplicate_set.begin(), remove_duplicate_set.end());
		}
		else
		{
			for (auto language : offer_id_map_to_detail_info[offer_id]->supported_languages)
			{
				if (!language.IsEmpty())
					other_region_languages_list.emplace_back(language);
			}
			auto func = [&supported_languages_list](const std::string &language_name) -> bool
			{
				return std::find(supported_languages_list.begin(), supported_languages_list.end(), language_name) != supported_languages_list.end();
			};
			other_region_languages_list.erase(std::remove_if(other_region_languages_list.begin(), other_region_languages_list.end(), func), other_region_languages_list.end());
		}
	}

	std::unordered_set<std::string> remove_duplicate_set(other_region_languages_list.begin(), other_region_languages_list.end());
	other_region_languages_list.clear();
	other_region_languages_list.assign(remove_duplicate_set.begin(), remove_duplicate_set.end());

	wxString current_supported_languages, other_region_languages;
	for (auto &language_name : supported_languages_list)
		if (!language_name.empty())
			current_supported_languages = current_supported_languages.IsEmpty() ? wxString(language_name) : current_supported_languages + "\n" + language_name;
	for (auto &language_name : other_region_languages_list)
		if (!language_name.empty())
			other_region_languages = other_region_languages.IsEmpty() ? wxString(language_name) : other_region_languages + "\n" + language_name;

	wxString all_supported_languages;
	all_supported_languages = other_region_languages_list.empty() ? current_supported_languages :
		languages::TranslateStaticText(current_language, "current_region") + "\n" + current_supported_languages + "\n\n" +
		languages::TranslateStaticText(current_language, "other_region") +"\n" + other_region_languages;
	m_staticText_languages->SetLabel(all_supported_languages);

	auto &current_offer_price = offer_path_with_prices[offer_path];
	std::multimap<double, std::string> convert_number_map_to_original_currency;
	wxString error_message;
	for (auto &[currency, price] : current_offer_price)
	{
		try
		{
			double converted_price = currency_api_ptr->ConvertCurrency(currency, selected_currency.ToStdString(), price);
			convert_number_map_to_original_currency.insert({ converted_price, currency });
		}
		catch (...)
		{
			wxString failed_message = languages::TranslateCurrencyName(current_language, currency)
				+ " " + languages::TranslateStaticText(current_language, "conversion_failed");
			error_message = "\n\n" + (error_message.IsEmpty() ? failed_message : error_message + "\n" + failed_message);
		}
	}

	wxString currency_order;
	for (auto &[converted_price, original_currency] : convert_number_map_to_original_currency)
	{
		std::stringstream double_to_string;
		double_to_string << round(converted_price * 100) / 100.0;
		wxString price_string = languages::TranslateCurrencyName(current_language, original_currency) + " "
			+ string_utitlies::remove_trailing_zeros_copy(std::to_string(current_offer_price[original_currency])) + " | "
			+ languages::TranslateCurrencyName(current_language, selected_currency) + " " + string_utitlies::remove_trailing_zeros_copy(double_to_string.str());
		currency_order = currency_order.IsEmpty() ? price_string : currency_order + "\n" + price_string;
	}

	m_staticText_currency_order->SetLabel(currency_order + error_message);
	Layout();
}

void wxPriceQueryForOriginUIFrame::OnClickSettingButton(wxCommandEvent& event)
{
	if (setting_dialog.ShowModal() != wxID_OK)
	{
		return;
	}

	languages::Language new_language;
	wxString new_currency;
	wxString new_origin_lng;
	wxString new_origin_country;
	setting_dialog.GetClientSettings(new_language, new_currency, new_origin_lng, new_origin_country);
	new_origin_lng.MakeLower();
	new_origin_lng[3] = toupper(new_origin_lng[3]);
	new_origin_lng[4] = toupper(new_origin_lng[4]);

	if (selected_currency != new_currency)
	{
		selected_currency = new_currency;
		m_staticText_currency_order->SetLabel("");
		m_staticText_languages->SetLabel("");
	}

	if (new_language != current_language || new_origin_lng != origin_lng || new_origin_country != origin_two_letter_country)
	{
		current_language = new_language;
		origin_lng = new_origin_lng;
		origin_two_letter_country = new_origin_country;
		
		m_treeListCtrl_Left->DeleteAllItems();
		m_treeListCtrl_Left->ClearColumns();
		m_treeListCtrl_ItemPrice->DeleteAllItems();
		m_treeListCtrl_ItemPrice->ClearColumns();
		m_staticText_currency_order->SetLabel("");
		m_staticText_languages->SetLabel("");
		SetupLabelText();
		run_idle_event_process = true;
	}
}

void wxPriceQueryForOriginUIFrame::OnTimerLoading(wxTimerEvent& event)
{
	progress_dialog = new wxProgressDialog(
		languages::TranslateStaticText(current_language, "wait"),
		languages::TranslateStaticText(current_language, "connect"),
		20, this, wxPD_SMOOTH | wxPD_APP_MODAL);
	this->Hide();
	progress_dialog->Show();
	progress_dialog->Pulse();
	progress_dialog->Refresh();
	progress_dialog->Update();
	queries_ptr = std::make_shared<query_tools::OriginQueries>();
	currency_api_ptr = std::make_shared<query_tools::CurrencyAPI>();

	setting_dialog.SetQueryClass(queries_ptr);
	setting_dialog.SetQueryClass(currency_api_ptr);

	std::thread loading_thread([&, this]()
		{
			if (setting_dialog.InitialiseConnections([&](std::string newmsg)
				{
					static int count = 0;
					progress_dialog->Pulse();
					progress_dialog->Refresh();
					progress_dialog->Update();
				}))
			{
				loading_completed = true;
				delete progress_dialog;
				progress_dialog = nullptr;
			}
			else
			{
				wxString message = languages::TranslateStaticText(current_language, "cannot_connect");
				wxString caption = languages::TranslateStaticText(current_language, "message");
				wxMessageBox(message, caption, wxICON_EXCLAMATION);
				delete progress_dialog;
				progress_dialog = nullptr;
				this->Destroy();
			}
		});
	loading_thread.detach();

	this->SetSize(FromDIP(wxSize(1250, 720)));
	this->CentreOnScreen();
	Layout();
}

void wxPriceQueryForOriginUIFrame::OnTimerSearch(wxTimerEvent& event)
{
	if (auto search_text = m_searchCtrl_Left->GetValue(); search_text.IsEmpty())
	{
		m_staticText_currency_order->SetLabel("");
		m_staticText_languages->SetLabel("");
		m_treeListCtrl_Left->DeleteAllItems();
		m_treeListCtrl_ItemPrice->DeleteAllItems();
		WriteCatalogueToTreeList(string_utitlies::to_underline_copy(origin_lng.ToStdString()));
	}
	else
	{
		m_staticText_currency_order->SetLabel("");
		m_staticText_languages->SetLabel("");
		m_treeListCtrl_Left->DeleteAllItems();
		m_treeListCtrl_ItemPrice->DeleteAllItems();
		auto offer_id_map_to_detail_info = queries_ptr->GetOfferIDMapping();
		auto offer_path_map_to_offer_id = queries_ptr->GetOfferIdFromOfferPathMap();

		auto item_level_top = m_treeListCtrl_Left->AppendItem(m_treeListCtrl_Left->GetRootItem(), languages::TranslateStaticText(current_language, "search_result"));
		for (auto &[offer_path, offer_ids] : offer_path_map_to_offer_id)
		{
			if (offer_ids.size() == 0) continue;
			WriteDetailDataToTreeList(origin_two_letter_country.ToStdString(), string_utitlies::to_underline_copy(origin_lng.ToStdString()), offer_id_map_to_detail_info, offer_ids, search_text.ToStdString());
		}
	}
}


void wxPriceQueryForOriginUIFrame::WriteCatalogueToTreeList(const std::string &language_code)
{
	auto catalogue_offer_path_mapping = queries_ptr->GetCatalogueMappingOfferPath();
	auto item_level_top = m_treeListCtrl_Left->AppendItem(m_treeListCtrl_Left->GetRootItem(), languages::TranslateStaticText(current_language, "all_games"));
	m_treeListCtrl_Left->SetItemText(item_level_top, 1, "Top Selection");
	for (auto genre_layer : catalogue_offer_path_mapping)
	{
		wxString label_name = queries_ptr->KeywordTranslation(language_code, genre_layer.first + "-label");
		if (label_name == genre_layer.first + "-label")
		{
			label_name = genre_layer.first;
			label_name.MakeCapitalized().Replace("-", " ");
		}
		if (genre_layer.first == "Other Genre")
		{
			label_name = languages::TranslateStaticText(current_language, "other_genre");
		}
		auto item_level_1 = m_treeListCtrl_Left->AppendItem(item_level_top, label_name);

		m_treeListCtrl_Left->SetItemText(item_level_1, 1, "genreFacetKey");
		m_treeListCtrl_Left->SetItemText(item_level_1, 2, genre_layer.first);
		for (auto franchise_layer : genre_layer.second)
		{
			label_name = queries_ptr->KeywordTranslation(language_code, franchise_layer.first + "-label");
			if (label_name == franchise_layer.first + "-label")
			{
				label_name = franchise_layer.first;
				label_name.MakeCapitalized().Replace("-", " ");
			}
			if (franchise_layer.first == "Other Franchise")
			{
				label_name = languages::TranslateStaticText(current_language, "other_franchise");
			}

			auto item_level_2 = m_treeListCtrl_Left->AppendItem(item_level_1, label_name);
			m_treeListCtrl_Left->SetItemText(item_level_2, 1, "franchiseFacetKey");
			m_treeListCtrl_Left->SetItemText(item_level_2, 2, franchise_layer.first);
			for (auto gamename_layer : franchise_layer.second)
			{
				label_name = queries_ptr->KeywordTranslation(language_code, gamename_layer.first + "-label");
				if (label_name == gamename_layer.first + "-label")
				{
					label_name = gamename_layer.first;
					label_name.MakeCapitalized().Replace("-", " ");
				}
				if (gamename_layer.first == "Other Game Type")
				{
					label_name = languages::TranslateStaticText(current_language, "other_game_type");
				}

				auto gamename_level = m_treeListCtrl_Left->AppendItem(item_level_2, label_name);
				m_treeListCtrl_Left->SetItemText(gamename_level, 1, "gameNameFacetKey");
				m_treeListCtrl_Left->SetItemText(gamename_level, 2, gamename_layer.first);
			}
		}
	}

	m_treeListCtrl_Left->Expand(item_level_top);
}

void wxPriceQueryForOriginUIFrame::WriteDetailDataToTreeList(const std::string &two_letter_country_code, const std::string &language_code, const std::map<std::string, std::shared_ptr<query_tools::GameDetailData>> &offer_id_map_to_detail_info, const std::unordered_set<std::string> &offer_ids, std::string filter_text)
{
	std::string offer_id;
	std::map<wxString, double> other_currency_price;

	for (auto &each_offer_id : offer_ids)
	{
		if (!filter_text.empty())
		{
			auto item_name = offer_id_map_to_detail_info.at(each_offer_id)->item_name[language_code];
			auto display_name = offer_id_map_to_detail_info.at(each_offer_id)->display_name[language_code];
			item_name.MakeLower();
			display_name.MakeLower();
			string_utitlies::to_lower(filter_text);
			if (!item_name.Contains(filter_text) && !display_name.Contains(filter_text)) continue;
		}

		if (auto &country_names = offer_id_map_to_detail_info.at(each_offer_id)->avaliable_country_two_letter; country_names.find(two_letter_country_code) != country_names.end())
		{
			offer_id = each_offer_id;
		}
		else
		{
			if (auto &price_discounted = offer_id_map_to_detail_info.at(each_offer_id)->price_discounted; price_discounted.size() > 0)
			{
				other_currency_price.insert(price_discounted.begin(), price_discounted.end());
			}
			else
			{
				auto &price_normal = offer_id_map_to_detail_info.at(each_offer_id)->price_normal;
				other_currency_price.insert(price_normal.begin(), price_normal.end());
			}
		}
	}
	if (offer_id.empty()) return;

	auto detail_info = offer_id_map_to_detail_info.at(offer_id);
	auto added_item = m_treeListCtrl_ItemPrice->AppendItem(m_treeListCtrl_ItemPrice->GetRootItem(), detail_info->display_name[language_code]);
	m_treeListCtrl_ItemPrice->SetItemText(added_item, 1, detail_info->offer_path);
	wxString platform_string = "Windows";
	if (auto &release_dates = detail_info->game_platforms_and_release_date; release_dates.size() > 1 && !release_dates["MAC"].IsEmpty())
	{
		platform_string = platform_string + " & macOS";
#ifdef __WXMSW__
		m_treeListCtrl_ItemPrice->SetItemText(added_item, 4, release_dates["PCWIN"]);
#else
		m_treeListCtrl_ItemPrice->SetItemText(added_item, 4, release_dates["MAC"]);
#endif // __WXMSW__	
	}
	else
	{
		m_treeListCtrl_ItemPrice->SetItemText(added_item, 4, release_dates["PCWIN"]);
	}
	m_treeListCtrl_ItemPrice->SetItemText(added_item, 3, platform_string);

	if (wxString extra_display_type = detail_info->extra_display_type[language_code]; extra_display_type.IsEmpty())
	{
		if (wxString display_type = detail_info->game_type_facet_key; display_type.MakeLower().IsEmpty())
		{
			m_treeListCtrl_ItemPrice->SetItemText(added_item, 2, detail_info->game_type_facet_key);
		}
		else
		{
			auto translated_label = queries_ptr->KeywordTranslation(language_code, (display_type + "-label").ToStdString());
			if (translated_label == display_type + "-label")
			{
				translated_label = queries_ptr->KeywordTranslation(language_code, display_type.ToStdString());
				if (translated_label == display_type && translated_label == "demos")
				{
					translated_label = queries_ptr->KeywordTranslation(language_code, "demo");
				}
			}
			m_treeListCtrl_ItemPrice->SetItemText(added_item, 2, translated_label);
		}

	}
	else m_treeListCtrl_ItemPrice->SetItemText(added_item, 2, extra_display_type);

	if (detail_info->price_discounted.size() > 0)
		m_treeListCtrl_ItemPrice->SetItemText(added_item, 5, languages::TranslateStaticText(current_language, "on_sale"));
	else
		m_treeListCtrl_ItemPrice->SetItemText(added_item, 5, "");


	auto update_price = [&](const std::map<wxString, double> &price_map)
	{
		for (auto &[currency, price] : price_map)
		{
			if (currency_name_map_to_column.find(currency) != currency_name_map_to_column.end())
			{
				m_treeListCtrl_ItemPrice->SetItemText(added_item, currency_name_map_to_column.at(currency), string_utitlies::remove_trailing_zeros_copy(std::to_string(price)));
				offer_path_with_prices[detail_info->offer_path.ToStdString()][currency.ToStdString()] = price;
			}
		}
	};

	update_price(detail_info->price_normal);
	update_price(other_currency_price);
	update_price(detail_info->price_discounted);
}

void wxPriceQueryForOriginUIFrame::SetupLabelText()
{
	this->SetTitle(languages::TranslateStaticText(current_language, "app_title"));

	m_searchCtrl_Left->SetLabel(languages::TranslateStaticText(current_language, "search"));
	
	m_treeListCtrl_Left->AppendColumn(languages::TranslateStaticText(current_language, "catalogue"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	m_treeListCtrl_Left->AppendColumn("entrytype", wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_HIDDEN);
	m_treeListCtrl_Left->AppendColumn("internalname", wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_HIDDEN);

	m_treeListCtrl_ItemPrice->AppendColumn(languages::TranslateStaticText(current_language, "title_name"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	m_treeListCtrl_ItemPrice->AppendColumn("OfferPath", wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_HIDDEN | wxCOL_RESIZABLE);
	m_treeListCtrl_ItemPrice->AppendColumn(languages::TranslateStaticText(current_language, "item_type"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE);
	m_treeListCtrl_ItemPrice->AppendColumn(languages::TranslateStaticText(current_language, "platform"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	m_treeListCtrl_ItemPrice->AppendColumn(languages::TranslateStaticText(current_language, "release_date"), wxCOL_WIDTH_DEFAULT, wxALIGN_CENTRE, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	m_treeListCtrl_ItemPrice->AppendColumn(languages::TranslateStaticText(current_language, "on_sale"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_CENTRE, wxCOL_RESIZABLE | wxCOL_SORTABLE);

	sbSizer_Prices->GetStaticBox()->SetLabel(languages::TranslateStaticText(current_language, "price_sorting"));
	sbSizerLanguages->GetStaticBox()->SetLabel(languages::TranslateStaticText(current_language, "supported_languages"));
	m_button_Settings->SetLabel(languages::TranslateStaticText(current_language, "settings"));

	if (auto currency_list = queries_ptr->GetCurrencyList(); currency_name_map_to_column.size() < currency_list.size())
	{
		for (auto &[country_name, currency] : currency_list)
		{
			currency_name_map_to_column[currency] = 0;
		}

		auto count = m_treeListCtrl_ItemPrice->GetColumnCount();
		for (auto &[currency_name, column_pos] : currency_name_map_to_column)
		{
			m_treeListCtrl_ItemPrice->AppendColumn(languages::TranslateCurrencyName(current_language, currency_name), wxCOL_WIDTH_DEFAULT, wxALIGN_CENTRE, wxCOL_RESIZABLE);
			column_pos = count++;
		}
	}
}
