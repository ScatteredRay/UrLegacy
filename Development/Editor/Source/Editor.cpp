// OtherGame.cpp : Defines the entry point for the application.
//

#include "wx/wx.h"
#include "Editor.h"

class UEditorApp : public wxApp
{
	virtual bool OnInit();
};

class UEdMainFrame : public wxFrame
{
public:
	UEdMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

enum
{
	ID_Quit = 1
};

BEGIN_EVENT_TABLE(UEdMainFrame, wxFrame)
	EVT_MENU(ID_Quit, UEdMainFrame::OnQuit)
END_EVENT_TABLE()

IMPLEMENT_APP(UEditorApp)

bool UEditorApp::OnInit()
{
	UEdMainFrame* Frame = new UEdMainFrame(TEXT("Ursa Editor"), wxPoint(50,50), wxSize(500,500));
	Frame->Show(TRUE);
	SetTopWindow(Frame);
	return true;
}

UEdMainFrame::UEdMainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
	wxMenu* menuFile = new wxMenu();

	menuFile->Append(ID_Quit, TEXT("E&xit"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, TEXT("&File"));
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText(TEXT("Ursa Editor"));
}

void UEdMainFrame::OnQuit(wxCommandEvent &event)
{
	Close(true);
}