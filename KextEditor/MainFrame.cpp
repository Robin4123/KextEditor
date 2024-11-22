#include "MainFrame.h"
#include <wx/richtext/richtextctrl.h>
#include "resource.h"
#include <wx/menu.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	filePath = ""; // Initializing the program with no open file

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

	wxMenu* about = new wxMenu();

	topMenu->Append(fileMenu, "File");
	topMenu->Append(about, "About");

	/* Text area creation */
	wxRichTextCtrl* text = new wxRichTextCtrl(panel, wxID_ANY, "text");
	text->Bind(wxEVT_TEXT, &MainFrame::insertedText, this);
	
	mainSizer->Add(text, 1, wxEXPAND); // Text area will take the rest of the program space
	panel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);	
	SetClientSize(1000, 600);

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
	}
	else {

	}
}

void MainFrame::OnExit(const wxCommandEvent& evt)
{
	// Check if saved before letting user close
	this->Close();
}

void MainFrame::New(const wxCommandEvent& evt)
{
	wxMessageBox("reached New");

}

void MainFrame::NewFile(const wxCommandEvent& evt)
{
	wxMessageBox("reached NewFile");

}

void MainFrame::OpenFile(const wxCommandEvent& evt)
{
	wxMessageBox("reached open");

}

void MainFrame::insertedText(wxCommandEvent& evt)
{
	wxRichTextCtrl* textCtrl = dynamic_cast<wxRichTextCtrl*>(evt.GetEventObject());

	if (textCtrl)
		textContent = textCtrl->GetValue();
}

