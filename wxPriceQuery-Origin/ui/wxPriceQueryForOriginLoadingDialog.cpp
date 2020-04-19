#include "wxPriceQueryForOriginLoadingDialog.h"

wxPriceQueryForOriginLoadingDialog::wxPriceQueryForOriginLoadingDialog( wxWindow* parent )
:
LoadingDialog( parent )
{

}

void wxPriceQueryForOriginLoadingDialog::SetDisplayText(wxString text)
{
	auto current_font = m_staticText->GetFont();
	current_font.SetPointSize(current_font.GetPointSize() * 2);
	m_staticText->SetFont(current_font);
	m_staticText->SetLabel(text);
}
