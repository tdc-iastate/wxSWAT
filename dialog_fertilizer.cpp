// dialog_fertilizer.cpp : implementation file
//

#include "../util/utility_afx.h"
#include <vector>
#include <map>
#include "resource.h"
#include "../util/dynamic_string.h"
#include "../util/list.h"
#include "../util/custom.h"
// #include "../util/edit_help_text.h"
#include "../util/list_field_help.h"
#ifdef _ODBC_TDC
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_field_set.h"
#endif
#include "dialog_fertilizer.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(dialog_fertilizer, wxDialog)
END_EVENT_TABLE()

dialog_fertilizer::dialog_fertilizer
	(wxWindow* pParent,
	SWATFertilizer *pest,
	list_field_help_swat *p_help_list)
	: wxDialog(pParent, wxID_ANY, L"Fertilizer")
{
	fertilizer = pest;
	help_list = p_help_list;

    wxBoxSizer *stack = new wxBoxSizer (wxVERTICAL);

	wxBoxSizer *row = new wxBoxSizer (wxHORIZONTAL);
	edit_description = new wxTextCtrl (this, wxID_ANY, L"");
	row->Add (edit_description);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_name = new wxTextCtrl (this, wxID_ANY, L"");
	row->Add (edit_name);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_fminn = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_fminn);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_fminp = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_fminp);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_forgn = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_forgn);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_forgp = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_forgp);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_fnh3n = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_fnh3n);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_bactpdb = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_bactpdb);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_bactlpb = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_bactlpb);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_bactkdb = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_bactkdb);
	stack->Add (row);

	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);

	edit_name->SetValue (fertilizer->Name);
	edit_description->SetValue (fertilizer->Description);
	edit_fminn->Set (fertilizer->MineralN);
	edit_fminp->Set (fertilizer->MineralP);
	edit_forgn->Set (fertilizer->OrganicN);
	edit_forgp->Set (fertilizer->OrganicP);
	edit_fnh3n->Set (fertilizer->MineralNAppliedasNH3);
	edit_bactpdb->Set (fertilizer->PersistentBacteria);
	edit_bactlpb->Set (fertilizer->LessPersistentBacteria);
	edit_bactkdb->Set (fertilizer->BacteriaPartitionCoefficient);
}

void dialog_fertilizer::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {
		wxString text;

		text = edit_name->GetValue ();
		strcpy_s (fertilizer->Name, 9, text.ToAscii ());
		text = edit_description->GetValue ();
		strcpy_s (fertilizer->Description, 256, text.ToAscii ());
		fertilizer->MineralN = edit_fminn->Get ();
		fertilizer->MineralP = edit_fminp->Get ();
		fertilizer->OrganicN = edit_forgn->Get ();
		fertilizer->OrganicP = edit_forgp->Get ();
		fertilizer->MineralNAppliedasNH3 = edit_fnh3n->Get ();
		fertilizer->PersistentBacteria = edit_bactpdb->Get ();
		fertilizer->LessPersistentBacteria = edit_bactlpb->Get ();
		fertilizer->BacteriaPartitionCoefficient = edit_bactkdb->Get ();
	}
	wxDialog::EndModal (retCode);
}

