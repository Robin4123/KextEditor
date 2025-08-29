#include "MainFrame.h"
#include <filesystem>
#include <wx/richtext/richtextctrl.h>
#include "resource.h"
#include <wx/menu.h>
#include <windows.h>
#include <wx/hyperlink.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	filePath = ""; // Initializing the program with no open file
	isSaved = true;
	initializer(title);
}

MainFrame::MainFrame(const wxString& title, const wxString& fileToOpen) : wxFrame(nullptr, wxID_ANY, title)
{
	filePath = fileToOpen; // Initializing the program with no open file
	isSaved = true;
	initializer(title + " | " + fileToOpen);
	loadContentsOfFile(fileToOpen);
}

void MainFrame::initializer(const wxString& title)
{
	this->SetTitle(title);

	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Top menu creaties*/
	wxMenuBar* topMenu = new wxMenuBar(0);
	SetMenuBar(topMenu);

	// Creating the File drop down menu
	wxMenu* fileMenu = new wxMenu();
	wxMenuItem* newFile = new wxMenuItem(fileMenu, wxID_ANY, "New | Ctrl + N");
	wxMenuItem* newWindow = new wxMenuItem(fileMenu, wxID_ANY, "New Window | Ctrl + Shift + N"); // Creating a seperate window for a new file
	wxMenuItem* openFile = new wxMenuItem(fileMenu, wxID_ANY, "Open | Ctrl + O"); // To choose a file to open
	wxMenuItem* saveFile = new wxMenuItem(fileMenu, wxID_ANY, "Save | Ctrl + S"); // Saving the file
	wxMenuItem* exit = new wxMenuItem(fileMenu, wxID_ANY, "Exit"); // Saving the file

	/* Binding actions to items */
	Bind(wxEVT_MENU, &MainFrame::New, this, newFile->GetId());
	Bind(wxEVT_MENU, &MainFrame::NewFile, this, newWindow->GetId());
	Bind(wxEVT_MENU, &MainFrame::OpenFile, this, openFile->GetId());
	Bind(wxEVT_MENU, &MainFrame::SaveFile, this, saveFile->GetId());
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, exit->GetId());


	/* Defining key shortcuts */
	wxAcceleratorEntry shortCuts[4];
	shortCuts[0].Set(wxACCEL_CTRL, (int)'S', saveFile->GetId(), saveFile); // Ctrl + S for save
	shortCuts[1].Set(wxACCEL_CTRL, (int)'N', newFile->GetId(), newFile); // Ctrl + N New file in same window
	shortCuts[2].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'N', newWindow->GetId(), newWindow); // Ctrl + Shift + N New file in seperate window
	shortCuts[3].Set(wxACCEL_CTRL, (int)'O', openFile->GetId(), openFile); // Ctrl + O Open file


	wxAcceleratorTable accelTable(4, shortCuts);
	SetAcceleratorTable(accelTable);


	/* Adding the items to the menu */
	fileMenu->Append(newFile);
	fileMenu->Append(newWindow);
	fileMenu->Append(openFile);
	fileMenu->Append(saveFile);
	fileMenu->AppendSeparator();
	fileMenu->Append(exit);

	wxMenu* help = new wxMenu();
	wxMenuItem* about = new wxMenuItem(help, wxID_ANY, "About");
	Bind(wxEVT_MENU, &MainFrame::About, this, about->GetId());
	help->Append(about);

	topMenu->Append(fileMenu, "File");
	topMenu->Append(help, "Help");

	/* Text area creation */
	m_richTextCtrl = new wxRichTextCtrl(panel, wxID_ANY, "text");
	m_richTextCtrl->Bind(wxEVT_TEXT, &MainFrame::insertedText, this);

	mainSizer->Add(m_richTextCtrl, 1, wxEXPAND); // Text area will take the rest of the program space
	panel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
	SetClientSize(1000, 600);
}

void MainFrame::loadContentsOfFile(const wxString& fileToOpen)
{
	m_richTextCtrl->LoadFile(fileToOpen, wxRICHTEXT_TYPE_TEXT);
}

void MainFrame::SaveFile(const wxCommandEvent& evt)
{
	if (filePath == "") {
		wxFrame* dirFrame = new wxFrame(this, wxID_ANY, "Directory Frame"); // Making a new window for the saving proccess
		wxFileCtrl* fileCtrl = new wxFileCtrl(dirFrame, wxID_ANY, wxEmptyString, wxEmptyString, wxString::FromAscii(wxFileSelectorDefaultWildcardStr), wxFC_SAVE);
		wxFileDialog* fileDial = new wxFileDialog(dirFrame, wxString::FromAscii(wxFileSelectorPromptStr), wxEmptyString, wxEmptyString, "Text files (*.txt) | *.txt", wxFD_SAVE);
		fileDial->ShowModal();


		wxFile file(fileDial->GetPath(), wxFile::write);
		if (!file.IsOpened()) {
			wxLogError("Cannot save current text file in file %s.", fileDial->GetPath());
			return;
		}
		file.Write(textContent);
		file.Close();
		filePath = fileDial->GetPath();
	}
	else {
		wxFile file(filePath, wxFile::write);
		if (!file.IsOpened()) {
			wxLogError("Cannot save current text file in file %s.", filePath);
			return;
		}
		file.Write(textContent);
		file.Close();
	}

	this->SetTitle("Kext editor | " + filePath);
	isSaved = false;

}

void MainFrame::OnExit(const wxCommandEvent& evt)
{
	if (!isSaved) {
		int response = wxMessageBox(wxT("The current file isn't saved\nAre you sure you want to exit?"), wxT("Exit"), wxYES_NO);
		if (response == wxYES) { this->Close(); }
	}
	this->Close();
}

void MainFrame::New(const wxCommandEvent& evt)
{
	if (!isSaved) {
		int response = wxMessageBox(wxT("The current file isn't saved\nAre you sure you want to exit?"), wxT("Exit"), wxYES_NO);
		if (response == wxYES) { 
			MainFrame* mf = new MainFrame("Kext Editor");
			mf->Show();
			mf->Center();
			this->Close(); 
		}
	}
}

void MainFrame::NewFile(const wxCommandEvent& evt)
{
	MainFrame* mf = new MainFrame("Kext Editor");
	mf->Show();
	mf->Center();
}

void MainFrame::OpenFile(const wxCommandEvent& evt)
{
	wxFrame* dirFrame = new wxFrame(this, wxID_ANY, "Directory Frame"); // Making a new window for the saving proccess
	wxFileCtrl* fileCtrl = new wxFileCtrl(dirFrame, wxID_ANY, filePath, wxEmptyString, wxString::FromAscii(wxFileSelectorDefaultWildcardStr), wxFC_OPEN);
	wxFileDialog* fileDial = new wxFileDialog(dirFrame, wxString::FromAscii(wxFileSelectorPromptStr), filePath, wxEmptyString, "Text files (*.txt) | *.txt", wxFD_OPEN);
	fileDial->ShowModal();
	dirFrame->Close();
	
	std::filesystem::path current_executable_path;
	wchar_t buffer[MAX_PATH];

	// Get path of the editor executable
	try {
		GetModuleFileNameW(NULL, buffer, MAX_PATH);
		current_executable_path = std::filesystem::path(buffer);
	}
	catch (const std::filesystem::filesystem_error& e)  {
		wxMessageBox(wxString::Format(wxT("Filesystem error: %s"),  wxString(e.what())), wxT("Error"), wxICON_ERROR);
	}
	
	auto result = ShellExecute(NULL, L"open", current_executable_path.c_str(), fileDial->GetPath().c_str(), NULL, SW_SHOWNORMAL); // Open the file using the Kext Editor
	
	if ((INT_PTR)result > 32) {
		this->Close(); // success
	}
	else {
		wxMessageBox("Failed to launch application.", "Error", wxICON_ERROR);
	}

	
}

void MainFrame::insertedText(wxCommandEvent& evt)
{
	wxRichTextCtrl* textCtrl = dynamic_cast<wxRichTextCtrl*>(evt.GetEventObject());

	if (textCtrl) { // TODO: CHECK FOR ACTUAL CHANGE
		textContent = textCtrl->GetValue();
		isSaved = false;
		this->SetTitle("Kext editor* | " + filePath); // Indication that the changed content isn't changed
	}
}

void MainFrame::About(const wxCommandEvent& evt)
{
	wxDialog* aboutDialog = new wxDialog(this, wxID_ANY, "About Kext Editor", wxDefaultPosition, wxSize(500, 300), wxDEFAULT_DIALOG_STYLE);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// Description text
	wxStaticText* text = new wxStaticText(aboutDialog, wxID_ANY,
		"This simple text editor was created by Daniel Kolesnikov as part of his computer science degree in 2025, serving as a small private project.\n\n"
		"For contact:",
		wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	text->Wrap(480);
	sizer->Add(text, 0, wxALL | wxEXPAND, 10);

	// GitHub link
	wxHyperlinkCtrl* githubLink = new wxHyperlinkCtrl(aboutDialog, wxID_ANY,
		"GitHub: Robin4123",
		"https://github.com/Robin4123");
	sizer->Add(githubLink, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	// Website link
	wxHyperlinkCtrl* siteLink = new wxHyperlinkCtrl(aboutDialog, wxID_ANY,
		"Website: www.danielkolesnikov.com",
		"https://www.danielkolesnikov.com");
	sizer->Add(siteLink, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	// OK button
	wxButton* okButton = new wxButton(aboutDialog, wxID_OK, "OK");
	sizer->Add(okButton, 0, wxALIGN_CENTER | wxBOTTOM, 10);

	aboutDialog->SetSizerAndFit(sizer);
	aboutDialog->Centre(wxBOTH);
	aboutDialog->ShowModal();


}

