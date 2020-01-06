#pragma once

#include <wx/wx.h>
#include <filesystem>

class Item : public wxBoxSizer
{
public:
	std::filesystem::path pathfile;
	uintmax_t size;
private:
	wxTextCtrl* nomefile;
	wxTextCtrl* sizefile;
	wxCheckBox* filemark;
public:
	Item(wxWindow* parent, const std::filesystem::directory_entry& entry);
	virtual ~Item();
	bool marked();
};
