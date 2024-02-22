#include "../util/utility_afx.h"
#include <vector>
// #include <wx/frame.h>
#include <wx/bookctrl.h>
#include <wx/propdlg.h>
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
#include "../util/dynamic_string.h"
#include "../util/custom.h"
#include "../util/filename_struct.h"
#include "../util/list.h"
#include "../util/list_field_help.h"
#include "propsheet_basin.h"
#include "page_basin_1.h"
#include "page_basin_2.h"
#include "page_basin_3.h"
#include "page_basin_4.h"
#include "page_basin_5.h"
#include "page_basin_6.h"
#include "page_basin_7.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(propsheet_basin, wxPropertySheetDialog)
	EVT_NOTEBOOK_PAGE_CHANGED (wxID_ANY, propsheet_basin::OnPageChanged) 
END_EVENT_TABLE()

propsheet_basin::propsheet_basin
	(wxWindow *parent,
	SWATControlRecord *control,
	swat_controller *controller,
	list_field_help_swat *p_help,
	odbc_database *p_db,
	const wxSize &size)
	: wxPropertySheetDialog ()
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);
	Create (parent, wxID_ANY, _("Sub-Basin"), wxDefaultPosition,
	wxDefaultSize, // wxSize (850, 650),
	wxDEFAULT_DIALOG_STYLE); // | (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxRESIZE_BORDER));


	CreateButtons(wxOK | wxCANCEL | (int) wxPlatform::IfNot (wxOS_WINDOWS_CE, wxHELP));

    wxBookCtrlBase* notebook = GetBookCtrl();

    page1 = new page_basin_1 (notebook, wxID_ANY, control, controller, p_help, p_db);
    page2 = new page_basin_2 (notebook, wxID_ANY, control, controller, p_help, p_db);
    page3 = new page_basin_3 (notebook, wxID_ANY, control, controller, p_help, p_db);
    page4 = new page_basin_4 (notebook, wxID_ANY, control, controller, p_help, p_db);
    page5 = new page_basin_5 (notebook, wxID_ANY, control, controller, p_help, p_db);
    page6 = new page_basin_6 (notebook, wxID_ANY, control, controller, p_help, p_db);
    page7 = new page_basin_7 (notebook, wxID_ANY, control, controller, p_help, p_db);

	notebook->AddPage (page1, wxT("1"));
	notebook->AddPage (page2, wxT("2"));
	notebook->AddPage (page3, wxT("3"));
	notebook->AddPage (page4, wxT("4"));
	notebook->AddPage (page5, wxT("5"));
	notebook->AddPage (page6, wxT("6"));
	notebook->AddPage (page7, wxT("7"));

    LayoutDialog();
}

void propsheet_basin::add_extra_buttons ()

// referring to wxWizard.AddButtonRow

{
	wxBoxSizer *buttonRow;

	wxWindow *next_button = FindWindowById (wxID_FORWARD, this);

	buttonRow = (wxBoxSizer *) next_button->GetSizer ();

    wxButton *button_ok;

    button_ok = new wxButton(this, wxOK, _("&Ok"));

	buttonRow->Insert (0, button_ok, 0, wxALL, 5);
	// buttonRow->Add (button_load_cfg, 0, wxALL, 5);
}


void propsheet_basin::OnPageChanged (wxNotebookEvent &eb)

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
