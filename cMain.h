#pragma once
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/spinctrl.h>
#include "Item.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
private:

	wxStaticText* m_selectf = nullptr;
	wxTextCtrl* m_texta1 = nullptr;
	wxButton* m_buttonpicker = nullptr;
	wxBoxSizer* m_mainsizer = nullptr;
	wxBoxSizer* m_pickersizer = nullptr;
	wxBoxSizer* m_filtersizer = nullptr;
	wxStaticText* m_filtertex = nullptr;
	wxDirDialog* m_fd = nullptr;
	wxSpinCtrl* m_spinsizefilter = nullptr;
	wxButton* m_buttoncheck = nullptr;
	wxScrolledWindow* m_listfiles = nullptr;
	wxBoxSizer* m_listsizer = nullptr;
	wxButton* m_buttonexclude = nullptr;
	wxStaticBoxSizer* m_bxlist = nullptr;
	std::vector<Item*> itens;

	void onButtonPickerClicked(wxCommandEvent& ev);
	void onButtonCheckClicked(wxCommandEvent& ev);
	void onButtonExcludeClicked(wxCommandEvent& ev);
};

