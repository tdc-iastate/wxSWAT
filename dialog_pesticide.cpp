// dialog_pesticide.cpp : implementation file
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
#include <odbcinst.h>
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"
#endif
#include "list_field_help_swat.h"
#include "dialog_pesticide.h"
#include "../i_swat_database/SWATPesticide.h"

BEGIN_EVENT_TABLE(dialog_pesticide, wxDialog)
END_EVENT_TABLE()

dialog_pesticide::dialog_pesticide
	(wxWindow* pParent,
	class SWATPesticide *pest,
	class list_field_help_swat *help)
	: wxDialog(pParent, wxID_ANY, L"Pesticide")
{
	pesticide = pest;
	help_list = help;

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
	edit_applicationefficiency = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_applicationefficiency);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_water_solubility = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_water_solubility);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_washofffraction = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_washofffraction);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_soilabsorptioncoefficient = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_soilabsorptioncoefficient);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_henrysconstant = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_henrysconstant);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_halflifeonfoliage = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_halflifeonfoliage);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_halflifeinsoil = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_halflifeinsoil);
	stack->Add (row);

	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);

	edit_name->SetValue (pesticide->Name);
	edit_description->SetValue (pesticide->Description);
	edit_applicationefficiency->Set (pesticide->ApplicationEfficiency);
	edit_water_solubility->Set (pesticide->WaterSolubility);
	edit_washofffraction->Set (pesticide->WashOffFraction);
	edit_soilabsorptioncoefficient->Set (pesticide->SoilAbsorptionCoefficient);
	edit_henrysconstant->Set  (pesticide->HenrysConstant);
	edit_halflifeonfoliage->Set (pesticide->HalfLifeOnFoliage);
	edit_halflifeinsoil->Set (pesticide->HalfLifeInSoil);
}

void dialog_pesticide::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {
		wxString text;

		text = edit_name->GetValue ();
		strcpy_s (pesticide->Name, 9, text.ToAscii ());
		text = edit_description->GetValue ();
		strcpy_s (pesticide->Description, 256, text.ToAscii ());

		pesticide->ApplicationEfficiency = edit_applicationefficiency->Get ();
		pesticide->WaterSolubility = edit_water_solubility->Get ();
		pesticide->WashOffFraction = edit_washofffraction->Get ();
		pesticide->SoilAbsorptionCoefficient = edit_soilabsorptioncoefficient->Get ();
		pesticide->HenrysConstant = edit_henrysconstant->Get ();
		pesticide->HalfLifeOnFoliage = edit_halflifeonfoliage->Get ();
		pesticide->HalfLifeInSoil = edit_halflifeinsoil->Get ();
	}
	wxDialog::EndModal (retCode);
}
