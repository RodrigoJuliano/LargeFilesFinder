#pragma once

#include <wx/wx.h>
#include "cMain.h"

class cApp : public wxApp
{
public:
	virtual bool OnInit();
private:
	cMain* m_main_frame = nullptr;
};

