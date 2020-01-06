#include "cMain.h"
#include <wx/wrapsizer.h>
#include <filesystem>

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Big files finder")
{
	SetMinSize({ 200,300 });
	// Create the widgets
	m_selectf = new wxStaticText(this, wxID_ANY, "Select the folder");
	m_buttonpicker = new wxButton(this, wxID_ANY, "...");
	m_filtertex = new wxStaticText(this, wxID_ANY, "Size (MB):");
	m_spinsizefilter = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, 16384L, 0, 1000000, 200);
	m_buttoncheck = new wxButton(this, wxID_ANY, "Check");
	m_texta1 = new wxTextCtrl(this, wxID_ANY);
	m_pickersizer = new wxBoxSizer(wxHORIZONTAL);
	m_mainsizer = new wxBoxSizer(wxVERTICAL);
	m_filtersizer = new wxBoxSizer(wxHORIZONTAL);
	m_listfiles = new wxScrolledWindow(this, wxID_ANY);
	m_listfiles->SetBackgroundColour(GetBackgroundColour());
	m_buttonexclude = new wxButton(this, wxID_ANY, "Delete marked");
	m_bxlist = new wxStaticBoxSizer(wxVERTICAL, this, "Files found");
	
	// Associate the widgets
	m_pickersizer->Add(m_texta1, 1);
	m_pickersizer->Add(m_buttonpicker, 0);

	m_filtersizer->Add(m_filtertex, 0, wxTOP | wxRIGHT, 12);
	m_filtersizer->Add(m_spinsizefilter, 1, wxTOP | wxRIGHT , 10);
	m_filtersizer->Add(m_buttoncheck, 1, wxTOP | wxLEFT, 10);

	m_mainsizer->Add(m_selectf, 0, wxALL | wxALIGN_CENTER_HORIZONTAL,10);
	m_mainsizer->Add(m_pickersizer, 0, wxGROW | wxLEFT | wxRIGHT, 10);
	m_mainsizer->Add(m_filtersizer, 0, wxGROW | wxLEFT | wxRIGHT, 10);
	m_bxlist->Add(m_listfiles, 1, wxALL | wxGROW, 5);
	m_mainsizer->Add(m_bxlist, 1, wxALL | wxGROW, 10);
	m_mainsizer->Add(m_buttonexclude, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

	this->SetSizer(m_mainsizer);

	// Set the events
	m_buttonpicker->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonPickerClicked, this);
	m_buttoncheck->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonCheckClicked, this);
	m_buttonexclude->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonExcludeClicked, this);
}

cMain::~cMain()
{
}

void cMain::onButtonPickerClicked(wxCommandEvent& ev)
{
	wxDirDialog* m_fd = new wxDirDialog(this, "Select the folder", wxEmptyString);

	// Creates a "open directory" dialog
	if (m_fd->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		m_texta1->SetLabelText(m_fd->GetPath());
	}

	// Clean up after ourselves
	m_fd->Destroy();
}

void cMain::onButtonCheckClicked(wxCommandEvent& ev) {
	itens.clear();
	m_listsizer = new wxBoxSizer(wxVERTICAL);
	m_listfiles->SetSizer(m_listsizer);
	m_listfiles->SetScrollRate(5, 5);

	std::string path = m_texta1->GetLabelText().ToStdString();
	if (std::filesystem::is_directory(path)) {
		std::filesystem::recursive_directory_iterator idir(path);
		for (auto& d : idir) {
			if (d.is_regular_file()) // && _access(path.c_str(), _A_NORMAL))
				if (d.file_size() > m_spinsizefilter->GetValue() * 1048576) {
					Item* bs = new Item(m_listfiles, d);
					itens.push_back(bs);
				}
		}
		std::sort(itens.begin(), itens.end(),
			[](Item* a, Item* b) {return a->size > b->size; });
		for (auto i : itens)
			m_listsizer->Add(i, 0, wxGROW);
		m_listfiles->FitInside();
		m_listsizer->Fit(m_listfiles);
		Layout();
	}
}

void cMain::onButtonExcludeClicked(wxCommandEvent& ev) {
	if (itens.empty())
		return;
	wxMessageDialog* d = new wxMessageDialog(this,
		"Delete the selected files?", wxMessageBoxCaptionStr, wxOK | wxCANCEL);
	if(d->ShowModal() == wxID_OK)
		for (auto i : itens) {
			if (i->marked()) {
				std::filesystem::remove(i->pathfile);
				m_listsizer->Remove(i);
			}
		}
	m_listfiles->FitInside();
	m_listsizer->Fit(m_listfiles);
	Layout();
}