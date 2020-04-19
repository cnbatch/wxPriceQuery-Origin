///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/srchctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treelist.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_TIMER_LOADING 1000
#define ID_TIMER_SEARCH 1001

///////////////////////////////////////////////////////////////////////////////
/// Class UIFrame
///////////////////////////////////////////////////////////////////////////////
class UIFrame : public wxFrame
{
	private:

	protected:
		wxSplitterWindow* m_splitter_Left;
		wxPanel* m_panel_Left;
		wxSearchCtrl* m_searchCtrl_Left;
		wxTreeListCtrl* m_treeListCtrl_Left;
		wxPanel* m_panelMiddle;
		wxTreeListCtrl* m_treeListCtrl_ItemPrice;
		wxPanel* m_panel_Right;
		wxPanel* m_panel_RightInner;
		wxStaticBoxSizer* sbSizer_Prices;
		wxStaticText* m_staticText_currency_order;
		wxStaticBoxSizer* sbSizerLanguages;
		wxStaticText* m_staticText_languages;
		wxButton* m_button_Settings;
		wxTimer m_timer_loading;
		wxTimer m_timer_search;

		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnTextInputSearch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreelistGameListSelectionChanged( wxTreeListEvent& event ) { event.Skip(); }
		virtual void OnTreelistGamePriceSelectionChanged( wxTreeListEvent& event ) { event.Skip(); }
		virtual void OnClickSettingButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimerLoading( wxTimerEvent& event ) { event.Skip(); }
		virtual void OnTimerSearch( wxTimerEvent& event ) { event.Skip(); }


	public:

		UIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxPriceQuery for Origin"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~UIFrame();

		void m_splitter_LeftOnIdle( wxIdleEvent& )
		{
			m_splitter_Left->SetSashPosition( 0 );
			m_splitter_Left->Disconnect( wxEVT_IDLE, wxIdleEventHandler( UIFrame::m_splitter_LeftOnIdle ), NULL, this );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class SettingDialog
///////////////////////////////////////////////////////////////////////////////
class SettingDialog : public wxDialog
{
	private:

	protected:
		wxPanel* m_panel_dialog;
		wxStaticBoxSizer* sbSizer_Setting_Interface;
		wxStaticText* m_staticText_interface_lng;
		wxChoice* m_choice_interface_lng;
		wxStaticText* m_staticText_my_currency;
		wxChoice* m_choice_currency;
		wxStaticBoxSizer* sbSizer_Setting_Origin;
		wxStaticText* m_staticText_game_lng;
		wxChoice* m_choice_game_lng;
		wxStaticText* m_staticText_Region;
		wxChoice* m_choice_Region;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;
		wxButton* m_sdbSizerButtonsCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnChoiceInterfaceLanguage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClockCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickOK( wxCommandEvent& event ) { event.Skip(); }


	public:

		SettingDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~SettingDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class LoadingDialog
///////////////////////////////////////////////////////////////////////////////
class LoadingDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText;

	public:

		LoadingDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxBORDER_NONE );
		~LoadingDialog();

};

