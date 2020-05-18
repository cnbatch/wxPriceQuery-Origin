///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "frame.h"

///////////////////////////////////////////////////////////////////////////

UIFrame::UIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerTop;
	bSizerTop = new wxBoxSizer( wxHORIZONTAL );

	m_splitter_Left = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter_Left->Connect( wxEVT_IDLE, wxIdleEventHandler( UIFrame::m_splitter_LeftOnIdle ), NULL, this );

	m_panel_Left = new wxPanel( m_splitter_Left, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLeft;
	bSizerLeft = new wxBoxSizer( wxVERTICAL );

	m_searchCtrl_Left = new wxSearchCtrl( m_panel_Left, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	m_searchCtrl_Left->ShowSearchButton( true );
	#endif
	m_searchCtrl_Left->ShowCancelButton( false );
	bSizerLeft->Add( m_searchCtrl_Left, 0, wxALL|wxEXPAND, 5 );

	m_treeListCtrl_Left = new wxTreeListCtrl( m_panel_Left, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE );

	bSizerLeft->Add( m_treeListCtrl_Left, 1, wxEXPAND | wxALL, 5 );


	m_panel_Left->SetSizer( bSizerLeft );
	m_panel_Left->Layout();
	bSizerLeft->Fit( m_panel_Left );
	m_panelMiddle = new wxPanel( m_splitter_Left, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerMiddle;
	bSizerMiddle = new wxBoxSizer( wxVERTICAL );

	m_treeListCtrl_ItemPrice = new wxTreeListCtrl( m_panelMiddle, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE );
	m_treeListCtrl_ItemPrice->SetMinSize( wxSize( 600,-1 ) );


	bSizerMiddle->Add( m_treeListCtrl_ItemPrice, 1, wxEXPAND | wxALL, 5 );


	m_panelMiddle->SetSizer( bSizerMiddle );
	m_panelMiddle->Layout();
	bSizerMiddle->Fit( m_panelMiddle );
	m_splitter_Left->SplitVertically( m_panel_Left, m_panelMiddle, 0 );
	bSizerTop->Add( m_splitter_Left, 1, wxEXPAND, 5 );

	m_panel_Right = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer_Right;
	bSizer_Right = new wxBoxSizer( wxVERTICAL );

	m_panel_RightInner = new wxPanel( m_panel_Right, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerTopRight;
	bSizerTopRight = new wxBoxSizer( wxVERTICAL );

	sbSizer_Prices = new wxStaticBoxSizer( new wxStaticBox( m_panel_RightInner, wxID_ANY, wxT("Prices Sorting") ), wxVERTICAL );

	m_staticText_currency_order = new wxStaticText( sbSizer_Prices->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText_currency_order->Wrap( -1 );
	sbSizer_Prices->Add( m_staticText_currency_order, 0, wxALL, 5 );


	bSizerTopRight->Add( sbSizer_Prices, 1, wxEXPAND, 5 );

	sbSizerLanguages = new wxStaticBoxSizer( new wxStaticBox( m_panel_RightInner, wxID_ANY, wxT("Supported Languages") ), wxVERTICAL );

	m_staticText_languages = new wxStaticText( sbSizerLanguages->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_languages->Wrap( -1 );
	sbSizerLanguages->Add( m_staticText_languages, 0, wxALL, 5 );


	bSizerTopRight->Add( sbSizerLanguages, 1, wxEXPAND, 5 );


	m_panel_RightInner->SetSizer( bSizerTopRight );
	m_panel_RightInner->Layout();
	bSizerTopRight->Fit( m_panel_RightInner );
	bSizer_Right->Add( m_panel_RightInner, 1, wxEXPAND | wxALL, 5 );

	m_button_Settings = new wxButton( m_panel_Right, wxID_ANY, wxT("Settings..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer_Right->Add( m_button_Settings, 0, wxALL|wxEXPAND, 5 );

	m_button_About = new wxButton( m_panel_Right, wxID_ANY, wxT("About"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer_Right->Add( m_button_About, 0, wxALL|wxEXPAND, 5 );


	m_panel_Right->SetSizer( bSizer_Right );
	m_panel_Right->Layout();
	bSizer_Right->Fit( m_panel_Right );
	bSizerTop->Add( m_panel_Right, 0, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizerTop );
	this->Layout();
	bSizerTop->Fit( this );
	m_timer_loading.SetOwner( this, ID_TIMER_LOADING );
	m_timer_search.SetOwner( this, ID_TIMER_SEARCH );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( UIFrame::OnIdle ) );
	m_searchCtrl_Left->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UIFrame::OnTextInputSearch ), NULL, this );
	m_treeListCtrl_Left->Connect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( UIFrame::OnTreelistGameListSelectionChanged ), NULL, this );
	m_treeListCtrl_ItemPrice->Connect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( UIFrame::OnTreelistGamePriceSelectionChanged ), NULL, this );
	m_button_Settings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UIFrame::OnClickSettingButton ), NULL, this );
	m_button_About->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UIFrame::OnClickAboutButton ), NULL, this );
	this->Connect( ID_TIMER_LOADING, wxEVT_TIMER, wxTimerEventHandler( UIFrame::OnTimerLoading ) );
	this->Connect( ID_TIMER_SEARCH, wxEVT_TIMER, wxTimerEventHandler( UIFrame::OnTimerSearch ) );
}

UIFrame::~UIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( UIFrame::OnIdle ) );
	m_searchCtrl_Left->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UIFrame::OnTextInputSearch ), NULL, this );
	m_treeListCtrl_Left->Disconnect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( UIFrame::OnTreelistGameListSelectionChanged ), NULL, this );
	m_treeListCtrl_ItemPrice->Disconnect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( UIFrame::OnTreelistGamePriceSelectionChanged ), NULL, this );
	m_button_Settings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UIFrame::OnClickSettingButton ), NULL, this );
	m_button_About->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UIFrame::OnClickAboutButton ), NULL, this );
	this->Disconnect( ID_TIMER_LOADING, wxEVT_TIMER, wxTimerEventHandler( UIFrame::OnTimerLoading ) );
	this->Disconnect( ID_TIMER_SEARCH, wxEVT_TIMER, wxTimerEventHandler( UIFrame::OnTimerSearch ) );

}

SettingDialog::SettingDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerDialog;
	bSizerDialog = new wxBoxSizer( wxVERTICAL );

	m_panel_dialog = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer_settings;
	bSizer_settings = new wxBoxSizer( wxVERTICAL );

	sbSizer_Setting_Interface = new wxStaticBoxSizer( new wxStaticBox( m_panel_dialog, wxID_ANY, wxT("Inteface Setting") ), wxVERTICAL );

	m_staticText_interface_lng = new wxStaticText( sbSizer_Setting_Interface->GetStaticBox(), wxID_ANY, wxT("Interface Langauge:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_interface_lng->Wrap( -1 );
	sbSizer_Setting_Interface->Add( m_staticText_interface_lng, 0, wxALL, 5 );

	wxArrayString m_choice_interface_lngChoices;
	m_choice_interface_lng = new wxChoice( sbSizer_Setting_Interface->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_interface_lngChoices, 0 );
	m_choice_interface_lng->SetSelection( 0 );
	sbSizer_Setting_Interface->Add( m_choice_interface_lng, 0, wxALL|wxEXPAND, 5 );

	m_staticText_my_currency = new wxStaticText( sbSizer_Setting_Interface->GetStaticBox(), wxID_ANY, wxT("My Currency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_my_currency->Wrap( -1 );
	sbSizer_Setting_Interface->Add( m_staticText_my_currency, 0, wxALL, 5 );

	wxArrayString m_choice_currencyChoices;
	m_choice_currency = new wxChoice( sbSizer_Setting_Interface->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_currencyChoices, 0 );
	m_choice_currency->SetSelection( 0 );
	sbSizer_Setting_Interface->Add( m_choice_currency, 0, wxALL|wxEXPAND, 5 );


	bSizer_settings->Add( sbSizer_Setting_Interface, 1, wxEXPAND, 5 );


	bSizer_settings->Add( 0, 15, 0, wxEXPAND, 5 );

	sbSizer_Setting_Origin = new wxStaticBoxSizer( new wxStaticBox( m_panel_dialog, wxID_ANY, wxT("Origin Parameters") ), wxVERTICAL );

	m_staticText_game_lng = new wxStaticText( sbSizer_Setting_Origin->GetStaticBox(), wxID_ANY, wxT("Language of Game List:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_game_lng->Wrap( -1 );
	sbSizer_Setting_Origin->Add( m_staticText_game_lng, 0, wxALL, 5 );

	wxArrayString m_choice_game_lngChoices;
	m_choice_game_lng = new wxChoice( sbSizer_Setting_Origin->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_game_lngChoices, 0 );
	m_choice_game_lng->SetSelection( 0 );
	sbSizer_Setting_Origin->Add( m_choice_game_lng, 0, wxALL|wxEXPAND, 5 );

	m_staticText_Region = new wxStaticText( sbSizer_Setting_Origin->GetStaticBox(), wxID_ANY, wxT("Region of Origin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_Region->Wrap( -1 );
	sbSizer_Setting_Origin->Add( m_staticText_Region, 0, wxALL, 5 );

	wxArrayString m_choice_RegionChoices;
	m_choice_Region = new wxChoice( sbSizer_Setting_Origin->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_RegionChoices, 0 );
	m_choice_Region->SetSelection( 0 );
	sbSizer_Setting_Origin->Add( m_choice_Region, 0, wxALL|wxEXPAND, 5 );


	bSizer_settings->Add( sbSizer_Setting_Origin, 1, wxEXPAND, 5 );


	bSizer_settings->Add( 0, 50, 0, wxEXPAND, 5 );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( m_panel_dialog, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( m_panel_dialog, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();

	bSizer_settings->Add( m_sdbSizerButtons, 0, wxEXPAND, 5 );


	m_panel_dialog->SetSizer( bSizer_settings );
	m_panel_dialog->Layout();
	bSizer_settings->Fit( m_panel_dialog );
	bSizerDialog->Add( m_panel_dialog, 1, wxEXPAND | wxALL, 15 );


	this->SetSizer( bSizerDialog );
	this->Layout();
	bSizerDialog->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_choice_interface_lng->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SettingDialog::OnChoiceInterfaceLanguage ), NULL, this );
	m_sdbSizerButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingDialog::OnClockCancel ), NULL, this );
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingDialog::OnClickOK ), NULL, this );
}

SettingDialog::~SettingDialog()
{
	// Disconnect Events
	m_choice_interface_lng->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SettingDialog::OnChoiceInterfaceLanguage ), NULL, this );
	m_sdbSizerButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingDialog::OnClockCancel ), NULL, this );
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SettingDialog::OnClickOK ), NULL, this );

}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_panel_about = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer_About;
	bSizer_About = new wxBoxSizer( wxVERTICAL );

	m_staticText_version = new wxStaticText( m_panel_about, wxID_ANY, wxT("version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_version->Wrap( -1 );
	bSizer_About->Add( m_staticText_version, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticText_name = new wxStaticText( m_panel_about, wxID_ANY, wxT("Project Codes on GitHub"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText_name->Wrap( -1 );
	bSizer_About->Add( m_staticText_name, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_hyperlink_project = new wxHyperlinkCtrl( m_panel_about, wxID_ANY, wxT("wxFB Website"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer_About->Add( m_hyperlink_project, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticline1 = new wxStaticLine( m_panel_about, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer_About->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_staticText_apis = new wxStaticText( m_panel_about, wxID_ANY, wxT("APIs of Exchange Rates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_apis->Wrap( -1 );
	bSizer_About->Add( m_staticText_apis, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_hyperlink_api1 = new wxHyperlinkCtrl( m_panel_about, wxID_ANY, wxT("wxFB Website"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer_About->Add( m_hyperlink_api1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_hyperlink_api2 = new wxHyperlinkCtrl( m_panel_about, wxID_ANY, wxT("wxFB Website"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer_About->Add( m_hyperlink_api2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panel_about->SetSizer( bSizer_About );
	m_panel_about->Layout();
	bSizer_About->Fit( m_panel_about );
	bSizer->Add( m_panel_about, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer );
	this->Layout();
	bSizer->Fit( this );

	this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
}
