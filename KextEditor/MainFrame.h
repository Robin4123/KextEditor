#pragma once
#include <wx/wx.h>
#include <wx/filectrl.h>
#include <wx/wfstream.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <wx/richtext/richtextctrl.h>


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	MainFrame(const wxString& title, const wxString& fileToOpen);

private:
	std::string filePath;
	wxString textContent;
	bool isSaved;
	wxRichTextCtrl* m_richTextCtrl;

	void initializer(const wxString& title);
	void loadContentsOfFile(const wxString& fileToOpen);

	void SaveFile(const wxCommandEvent& evt);
	void OnExit(const wxCommandEvent& evt);
	void New(const wxCommandEvent& evt);
	void NewFile(const wxCommandEvent& evt);
	void OpenFile(const wxCommandEvent& evt);
	void insertedText(wxCommandEvent& evt);
	void About(const wxCommandEvent& evt);
};

