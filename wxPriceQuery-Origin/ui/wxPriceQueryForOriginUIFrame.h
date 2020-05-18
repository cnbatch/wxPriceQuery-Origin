#ifndef __wxPriceQueryForOriginUIFrame__
#define __wxPriceQueryForOriginUIFrame__

/**
@file
Subclass of UIFrame, which is generated by wxFormBuilder.
*/

#include "frame.h"

//// end generated include

#include <wx/progdlg.h>
#include <memory>
#include "wxPriceQueryForOriginSettingDialog.h"
#include "wxPriceQueryForOriginAboutDialog.h"
#include "../query_tools/origin_tools/OriginQueries.hpp"
#include "../query_tools/CURLGlobalThread.hpp"
#include "../query_tools/exchange_rate/CurrencyAPI.hpp"

/** Implementing UIFrame */
class wxPriceQueryForOriginUIFrame : public UIFrame
{
	protected:
		// Handlers for UIFrame events.
		void OnIdle( wxIdleEvent& event );
		void OnTextInputSearch( wxCommandEvent& event );
		void OnTreelistGameListSelectionChanged( wxTreeListEvent& event );
		void OnTreelistGamePriceSelectionChanged( wxTreeListEvent& event );
		void OnClickSettingButton( wxCommandEvent& event );
		void OnClickAboutButton( wxCommandEvent& event );
		void OnTimerLoading( wxTimerEvent& event );
		void OnTimerSearch( wxTimerEvent& event );
	public:
		/** Constructor */
		wxPriceQueryForOriginUIFrame( wxWindow* parent );
	//// end generated class members



private:
	wxPriceQueryForOriginAboutDialog about_dialog = wxPriceQueryForOriginAboutDialog(this);
	wxPriceQueryForOriginSettingDialog setting_dialog = wxPriceQueryForOriginSettingDialog(this);
	wxProgressDialog *progress_dialog = nullptr;
	languages::Language current_language;
	wxString selected_currency;
	wxString origin_lng;
	wxString origin_two_letter_country;
	bool run_idle_event_process = false;
	bool loading_completed = false;
	std::map<wxString, int> currency_name_map_to_column;
	std::shared_ptr<query_tools::OriginQueries> queries_ptr;
	std::shared_ptr<query_tools::CurrencyAPI> currency_api_ptr;
	query_tools::CURLGlobalThread curl_global_thread;
	std::map<std::string, std::map<std::string, double>> offer_path_with_prices;

	void WriteCatalogueToTreeList(const std::string &language_code);
	void WriteDetailDataToTreeList(const std::string &two_letter_country_code, const std::string &language_code,
		const std::map<std::string, std::shared_ptr<query_tools::GameDetailData>> &offer_id_map_to_detail_info,
		const std::unordered_set<std::string> &offer_ids, std::string filter_text = std::string());
	void SetupLabelText();



};

#endif // __wxPriceQueryForOriginUIFrame__
