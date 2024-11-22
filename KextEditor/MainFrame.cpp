#include "MainFrame.h"
#include <wx/richtext/richtextctrl.h>
#include "resource.h"
#include <wx/menu.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Top menu creaties*/
	wxMenuBar* topMenu = new wxMenuBar(0);
	SetMenuBar(topMenu);

	// Creating the File drop down menu
	wxMenu* fileMenu = new wxMenu();
	wxMenuItem* newFile = new wxMenuItem(fileMenu, wxID_ANY, "New");
	wxMenuItem* newWindow = new wxMenuItem(fileMenu, wxID_ANY, "New Window"); // Creating a seperate window for a new file
	wxMenuItem* openFile = new wxMenuItem(fileMenu, wxID_ANY, "Open"); // To choose a file to open
	wxMenuItem* saveFile = new wxMenuItem(fileMenu, wxID_ANY, "Save"); // Saving the file
	wxMenuItem* exit = new wxMenuItem(fileMenu, wxID_ANY, "Exit"); // Saving the file

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



	mainSizer->Add(text, 1, wxEXPAND); // Text area will take the rest of the program space
	panel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);	
	SetClientSize(1000, 600);

}

