#ifdef _WIN32
#include <winsock2.h>
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "ui/frame.h"
#include "ui/wxPriceQueryForOriginUIFrame.h"
#include "ui/icon.xpm"
//#include <wx/msw/wx.rc>

class PriceQueryApp : public wxApp
{
public:
	virtual bool OnInit() wxOVERRIDE;
};

wxIMPLEMENT_APP(PriceQueryApp);

bool PriceQueryApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	
	wxPriceQueryForOriginUIFrame *frame = new wxPriceQueryForOriginUIFrame(nullptr);
	wxIcon app_icon(icon_xpm);
	frame->SetIcon(app_icon);
	frame->Show(true);

	return true;
}
