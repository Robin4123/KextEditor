#include "Kext.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(Kext);

bool Kext::OnInit() {
	MainFrame* mainFrame = new MainFrame("Kext Editor");
	mainFrame->Show();
	mainFrame->Center();
	return true;
}