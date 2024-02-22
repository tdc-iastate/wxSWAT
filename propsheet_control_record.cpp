#include "../util/utility_afx.h"
#include <vector>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/filename.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/propdlg.h>
#include "../util/dynamic_string.h"
#include "../util/custom.h"
#include "../util/filename_struct.h"
#include "propsheet_control_record.h"
// #include "bitmaps/propsheet_control_record.xpm"
#include "page_control_record.h"
#include "page_control_weather.h"
#include "page_control_details.h"


BEGIN_EVENT_TABLE(propsheet_control_record, wxPropertySheetDialog)
	EVT_NOTEBOOK_PAGE_CHANGED (wxID_ANY, propsheet_control_record::OnPageChanged) 
END_EVENT_TABLE()

propsheet_control_record::propsheet_control_record
	(wxWindow *parent,
	SWATControlRecord *control,
	swat_controller *controller,
	list_field_help_swat *help_list,
	odbc_database *db,
	const wxSize &size)
	: wxPropertySheetDialog ()
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    // Create (frame, wxID_ANY, wxT("i_SWAT"), wxBitmap(wiztest_xpm), wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    // Create (frame, wxID_ANY, wxT("i_SWAT"), wxNullBitmap, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	Create (parent, wxID_ANY, _("Control Record"), wxDefaultPosition, size, wxDEFAULT_DIALOG_STYLE); // | (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxRESIZE_BORDER));

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
	CreateButtons(wxOK | wxCANCEL | (int) wxPlatform::IfNot (wxOS_WINDOWS_CE, wxHELP));

    wxBookCtrlBase* notebook = GetBookCtrl();

    page1 = new page_control_record (notebook, control, controller, help_list, db);
    page2 = new page_control_weather (notebook, control);
	page3 = new page_control_details (notebook, control);

	notebook->AddPage (page1, wxT("Control"), true, -1);
	notebook->AddPage (page2, wxT("Weather"), false, -1);
	notebook->AddPage (page3, wxT("Details"), false, -1);

    /*
	page1->SetNext(page2);
	page2->SetPrev (page1);
	page2->SetNext (page3);
	page3->SetPrev (page2);
	*/

    LayoutDialog();
}

wxNotebookPage *propsheet_control_record::GetFirstPage() const
{
	return page1;
}

void propsheet_control_record::OnPageChanged
	(wxNotebookEvent &eb)

{
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
