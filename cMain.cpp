#include "cMain.h"
#include <filesystem>

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Big files finder")
{
	SetMinSize({ 200,300 });
	// Create the widgets
	m_selectf = new wxStaticText(this, wxID_ANY, "Select the folder");
	m_buttonpicker = new wxButton(this, wxID_ANY, "...");
	m_filtertex = new wxStaticText(this, wxID_ANY, "Min size (MB):");
	m_spinsizefilter = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, 16384L, 0, 1000000, 200);
	m_buttoncheck = new wxButton(this, wxID_ANY, "Check");
	m_texta1 = new wxTextCtrl(this, wxID_ANY);
	m_pickersizer = new wxBoxSizer(wxHORIZONTAL);
	m_mainsizer = new wxBoxSizer(wxVERTICAL);
	m_filtersizer = new wxBoxSizer(wxHORIZONTAL);
	m_buttonexclude = new wxButton(this, wxID_ANY, "Delete marked");
	m_listtitle = new wxStaticText(this, wxID_ANY, "Files found");
	m_listctrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES);
	m_listctrl->EnableCheckBoxes();
	// First column for the checkboxs
	long c0 = m_listctrl->AppendColumn("All", wxLIST_FORMAT_LEFT, 28);
	long c1 = m_listctrl->AppendColumn("File", wxLIST_FORMAT_LEFT, 263);
	long c2 = m_listctrl->AppendColumn("Size", wxLIST_FORMAT_LEFT, 70);
	
	// set the sizers
	m_pickersizer->Add(m_texta1, 1);
	m_pickersizer->Add(m_buttonpicker, 0);
	m_filtersizer->Add(m_filtertex, 0, wxTOP | wxRIGHT, 12);
	m_filtersizer->Add(m_spinsizefilter, 1, wxTOP | wxRIGHT , 10);
	m_filtersizer->Add(m_buttoncheck, 1, wxTOP | wxLEFT, 10);
	m_mainsizer->Add(m_selectf, 0, wxALL | wxALIGN_CENTER_HORIZONTAL,10);
	m_mainsizer->Add(m_pickersizer, 0, wxGROW | wxLEFT | wxRIGHT, 10);
	m_mainsizer->Add(m_filtersizer, 0, wxGROW | wxLEFT | wxRIGHT, 10);
	m_mainsizer->Add(m_listtitle, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
	m_mainsizer->Add(m_listctrl, 1, wxGROW | wxLEFT | wxRIGHT, 10);
	m_mainsizer->Add(m_buttonexclude, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

	SetSizer(m_mainsizer);

	// Set the events
	m_buttonpicker->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonPickerClicked, this);
	m_buttoncheck->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonCheckClicked, this);
	m_buttonexclude->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonExcludeClicked, this);
	m_listctrl->Bind(wxEVT_LIST_COL_CLICK, &cMain::onListColClicked, this);
}

cMain::~cMain()
{
}

void cMain::onButtonPickerClicked(wxCommandEvent& ev)
{
	// Creates a "open directory" dialog
	wxDirDialog* m_fd = new wxDirDialog(this, "Select the folder", wxEmptyString);
	// if the user click "Open" instead of "Cancel"
	if (m_fd->ShowModal() == wxID_OK)
	{
		m_texta1->SetLabelText(m_fd->GetPath());
	}

	// Clean up after ourselves
	m_fd->Destroy();
}

// fuction to compare 2 itens (used by SortItems)
static int wxCALLBACK Compare(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
	std::vector<Item*>* itens = reinterpret_cast<std::vector<Item*>*>(sortData);
	Item* i1 = (*itens)[static_cast<size_t>(item1)];
	Item* i2 = (*itens)[static_cast<size_t>(item2)];

	return int(i2->size - i1->size);
}

void cMain::onButtonCheckClicked(wxCommandEvent& ev) {
	// clean the actual data for a new search
	itens.clear();
	m_listctrl->DeleteAllItems();

	std::string path = m_texta1->GetLabelText().ToStdString();
	if (std::filesystem::is_directory(path)) {
		std::filesystem::recursive_directory_iterator idir(path);
		// percur all directories and subs
		for (auto& d : idir) {
			if (d.is_regular_file()) // && _access(path.c_str(), _A_NORMAL))
				if (d.file_size() > long(m_spinsizefilter->GetValue()) * 1048576L) {
					Item* bs = new Item(d);
					itens.push_back(bs);
					// First column is empty (for the checkbox)
					long in0 = m_listctrl->InsertItem(itens.size()-1, "");
					// add the file name
					m_listctrl->SetItem(in0, 1, bs->pathfile.filename().string());
					// add the file size
					m_listctrl->SetItem(in0, 2, std::to_string(bs->size / 1048576) + "MB");
					// save the item position in the item (to ordering)
					m_listctrl->SetItemData(in0, (long)(itens.size()-1));
				}
		}

		m_listctrl->SortItems(Compare, reinterpret_cast<wxIntPtr>(&itens));
	}
}

void cMain::onButtonExcludeClicked(wxCommandEvent& ev) {
	if (itens.empty())
		return;
	
	// Show dialog box to confirm
	wxMessageDialog* d = new wxMessageDialog(this,
		"Delete the selected files?", wxMessageBoxCaptionStr, wxOK | wxCANCEL);
	// if the user confirm
	if (d->ShowModal() == wxID_OK) {
		// start by the last because removing one item from the
		// list the itens are moved
		for (int i = m_listctrl->GetItemCount()-1; i >= 0; i--)
			// verify if the file is selected
			if (m_listctrl->IsItemChecked(i)) {
				// delete the file from disk and itens
				// and save the position on the list
				size_t pos = static_cast<size_t>(m_listctrl->GetItemData(i));
				std::filesystem::remove(itens[pos]->pathfile);
				delete itens[pos];
				itens[pos] = nullptr;
				m_listctrl->DeleteItem(i);
			}
	}
}

void cMain::onListColClicked(wxListEvent& ev)
{
	if (ev.m_col == 0) {
		allmarked = !allmarked;
		for (long i = m_listctrl->GetItemCount() - 1; i >= 0; i--)
			m_listctrl->CheckItem(i, allmarked);
	}
}
