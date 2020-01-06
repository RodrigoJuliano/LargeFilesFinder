#include "Item.h"

Item::Item(wxWindow* parent, const std::filesystem::directory_entry& entry)
	:
	wxBoxSizer(wxHORIZONTAL),
	pathfile(entry.path()),
	size(entry.file_size())
{
	nomefile = new wxTextCtrl(parent, wxID_ANY, pathfile.filename().generic_string());
	sizefile = new wxTextCtrl(parent, wxID_ANY, std::to_string(size / 1048576) + "MB");
	filemark = new wxCheckBox(parent, wxID_ANY, "");
	
	nomefile->SetEditable(false);
	sizefile->SetEditable(false);

	Add(filemark, 0, wxTOP | wxRIGHT, 4);
	Add(nomefile, 1);
	Add(sizefile, 0);

	//nomefile->SetToolTip(pathfile.string()); // too laggy
}

Item::~Item()
{
	delete nomefile;
	delete sizefile;
	delete filemark;
}

bool Item::marked()
{
	return filemark->GetValue();
}
