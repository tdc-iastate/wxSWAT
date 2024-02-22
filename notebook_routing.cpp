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
#include "../util/dynamic_string.h"
#include "../util/custom.h"
#include "../util/filename_struct.h"
#include "notebook_routing.h"
#include "page_routing_node.h"
#include "page_routing_output.h"

BEGIN_EVENT_TABLE(notebook_routing, wxNotebook)
	EVT_NOTEBOOK_PAGE_CHANGED (wxID_ANY, notebook_routing::OnPageChanged) 
END_EVENT_TABLE()

notebook_routing::notebook_routing
	(wxWindow *parent,
	swat_routing_node *node,
	SWATControlRecord *control,
	swat_controller *controller,
	const wxSize &size)
	: wxNotebook (parent, wxID_ANY, wxDefaultPosition, size)
{
    page1 = new page_routing_node (this, wxID_ANY, node, control, controller);
    page2 = new page_routing_output (this, wxID_ANY, node, control);

	AddPage (page1, wxT("Node"));
	AddPage (page2, wxT("Output"));

    page1->SetNext(page2);
	page2->SetPrev (page1);

}

wxNotebookPage *notebook_routing::GetFirstPage() const
{
	return page1;
}

void notebook_routing::add_extra_buttons ()

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


void notebook_routing::OnPageChanged (wxNotebookEvent &e)

{
	switch (e.GetOldSelection ()) {
		case 0:
			page1->read_controls ();
			break;
		case 1:
			page2->read_controls ();
	}
}
