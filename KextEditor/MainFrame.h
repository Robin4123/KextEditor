#pragma once
#include <wx/wx.h>
#include <wx/filectrl.h>
#include <wx/wfstream.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <wx/richtext/richtextctrl.h>

using namespace std;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	string filePath;
	wxString textContent;
	void SaveFile(const wxCommandEvent& evt);
	void OnExit(const wxCommandEvent& evt);
	void New(const wxCommandEvent& evt);
	void NewFile(const wxCommandEvent& evt);
	void OpenFile(const wxCommandEvent& evt);

	void insertedText(wxCommandEvent& evt);
};

