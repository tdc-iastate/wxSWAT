#include "../util/utility_afx.h"
#include <vector>
#include <wx/frame.h>
#include <wx/bookctrl.h>
#include <wx/propdlg.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/filename.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include "../util/dynamic_string.h"
#include "../util/custom.h"
#include "../util/filename_struct.h"
#include "../util/list.h"
#include "../util/list_field_help.h"
#include "propsheet_hru.h"
#include "page_hru.h"
#include "page_hru_2.h"
#include "page_hru_3.h"
#include "page_hru_management.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(propsheet_hru, wxPropertySheetDialog)
	EVT_NOTEBOOK_PAGE_CHANGED (wxID_ANY, propsheet_hru::OnPageChanged) 
END_EVENT_TABLE()

propsheet_hru::propsheet_hru
	(wxWindow *parent,
	HRU_record *hru,
	SubBasin *basin,
	SWATControlRecord *control,
	swat_controller *controller,
	list_field_help_swat *p_help,
	class odbc_database *p_db,
	const wxSize &size)
	: wxPropertySheetDialog ()
{
    // SetExtraStyle(wxWIZARD_EX_HELPBUTTON);

    // Create (frame, wxID_ANY, wxT("i_SWAT"), wxBitmap(wiztest_xpm), wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    // Create (frame, wxID_ANY, wxT("i_SWAT"), wxNullBitmap, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	// add_extra_buttons ();

    // SetIcon(wxICON(sample));

	/*
    // Allow the bitmap to be expanded to fit the page height
    if (frame->GetMenuBar()->IsChecked(Wizard_ExpandBitmap))
        SetBitmapPlacement(wxWIZARD_VALIGN_CENTRE);

    // Enable scrolling adaptation
    if (frame->GetMenuBar()->IsChecked(Wizard_LargeWizard))
        SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
	*/

    // a wizard page may be either an object of predefined class
	/*
    // ... or a derived class
    wxValidationPage *page4 = new wxValidationPage(this);

    // set the page order using a convenience function - could also use
    // SetNext/Prev directly as below
    wxWizardPageSimple::Chain(page3, page4);

	*/
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);
	Create (parent, wxID_ANY, _("HRU"), wxDefaultPosition, wxSize (850, 650), wxDEFAULT_DIALOG_STYLE); // | (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxRESIZE_BORDER));


	CreateButtons(wxOK | wxCANCEL | (int) wxPlatform::IfNot (wxOS_WINDOWS_CE, wxHELP));

    wxBookCtrlBase* notebook = GetBookCtrl();

    page1 = new page_hru (notebook, wxID_ANY, hru, basin, control, controller, p_help, p_db);
    page2 = new page_hru_2 (notebook, wxID_ANY, hru, basin, control, controller, p_help, p_db);
    page3 = new page_hru_3 (notebook, wxID_ANY, hru, basin, control, controller);
	/*
    page4 = new page_hru_gw (this, wxID_ANY, basin);
	*/
    page5 = new page_hru_management (notebook, wxID_ANY, hru, basin, control, controller);
	/*
    page6 = new page_hru_output (this, wxID_ANY, basin);
    page7 = new page_hru_pothole (this, wxID_ANY, basin);
    page8 = new page_hru_tcp (this, wxID_ANY, basin);
	*/

	notebook->AddPage (page1, wxT("HRU"));
	notebook->AddPage (page2, wxT("Urban && Irrigation"));
	notebook->AddPage (page3, wxT("Climate"));
	/*
	AddPage (page4, wxT("Elevation"));
	*/
	notebook->AddPage (page5, wxT("Management"));

	/*
    page1->SetNext(page2);
	page2->SetPrev (page1);
	page2->SetNext (page3);
	page3->SetPrev (page2);
	page3->SetNext (page5);
	page4->SetPrev (page3);
	page4->SetNext (page5);
	page5->SetPrev (page3);
	*/

    LayoutDialog();
}

void propsheet_hru::add_extra_buttons ()

// referring to wxWizard.AddButtonRow

{
	wxBoxSizer *buttonRow;

	/*
	{
		wxSizer *windowSizer = this->GetSizer ();
		wxBoxSizer *mainColumn;
		// Find the wxBoxSizer installed by AddButtonRow, when called by wxWizard.DoCreateControls
		// This would be much easier if AddButtonRow were virtual.

		// Sizer is windowSizer wxBoxSizer, 
		// to which is added mainColumn wxBoxSizer
		// then AddBitmapRow
		// then AddButtonRow

		wxSizerItemList::iterator iter_window, iter_column;
		wxSizerItemList children;
		children = windowSizer->GetChildren ();

		iter_window = children.begin();
		// ++iter_window;
		mainColumn = (wxBoxSizer *) (*iter_window)->GetSizer ();
		children = mainColumn->GetChildren (); // size of children is 5
		iter_column = children.begin (); // wxBoxSizer m_sizerBmpAndPage 
		++iter_column; // wxSizerItem 
		++iter_column; // wxStaticLine
		++iter_column; // wxSizerItem 
		++iter_column;
		buttonRow = (wxBoxSizer *) (*iter_column)->GetSizer ();
	}
	*/

	wxWindow *next_button = FindWindowById (wxID_FORWARD, this);

	buttonRow = (wxBoxSizer *) next_button->GetSizer ();

    wxButton *button_ok;

    button_ok = new wxButton(this, wxOK, _("&Ok"));

	buttonRow->Insert (0, button_ok, 0, wxALL, 5);
	// buttonRow->Add (button_load_cfg, 0, wxALL, 5);
}


void propsheet_hru::OnPageChanged (wxNotebookEvent &eb)

{
	/*
	switch (e.GetOldSelection ()) {
		case 0:
			page1->read_controls ();
	}
	*/
	size_t old_page_number = (size_t) eb.GetOldSelection ();
	if ((old_page_number >= 0)
	&& (old_page_number < GetBookCtrl ()->GetPageCount ())) {
		// First call occurs as dialog opens, old_page_number is a very large integer
		wxWindow *old_page = GetBookCtrl ()->GetPage (old_page_number);
		old_page->TransferDataFromWindow ();
	}

	wxWindow *new_page = GetBookCtrl ()->GetPage (eb.GetSelection ());
	new_page->TransferDataToWindow ();
}
