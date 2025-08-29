#include "Kext.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(Kext);

bool Kext::OnInit() {

	if (argc > 1) {
		wxString fileToOpen = argv[1];
		MainFrame* mainFrame = new MainFrame("Kext Editor", fileToOpen);
		mainFrame->Show();
		mainFrame->Center();
		return true;
		// Open the file
	}
	MainFrame* mainFrame = new MainFrame("Kext Editor");
	mainFrame->Show();
	mainFrame->Center();
	return true;
}