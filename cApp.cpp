#include "cApp.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit()
{
	m_main_frame = new cMain();
	m_main_frame->Show();
	return true;
}
