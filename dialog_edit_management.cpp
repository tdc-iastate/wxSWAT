// dialog_edit_management.cpp : implementation file
//

#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include "resource.h"
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/custom.h"
#include "../util/device_coordinate.h"
// #include "../util/edit_help_text.h"
#include "../util/list_field_help.h"
#ifdef _ODBC_TDC
#include <odbcinst.h>
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_field_set.h"
#endif
#include "dialog_edit_management.h"
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATSoilLayers.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATManagement.h"
#include "../i_swat_database/SWATTemporalConservationPractices.h"
#include "../i_swat_database/rowset_hru.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATWeatherHistorical.h"
#include "../i_swat_database/SWATWeatherByMonth.h"
#include "../i_swat_database/SWATWeatherStations.h"
#include "../i_swat_database/swat_weather_list.h"
#include "../i_swat_database/rowset_subbasin.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesAPEX.h"
#include "../i_swat_database/swat_routing_node.h"
#include "../i_swat_database/SWATOutputHRUAnnual.h"
#include "../i_swat_database/SWATOutputHRUMonthly.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
#include "../i_swat_database/SWATOutputHRUDaily.h"
#include "../i_swat_database/output_column_position.h"
#include "../i_swat_database/swat_output.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"
#include "../i_swat_database/swat_controller.h"
#include "../i_swat_database/SWATSepticWaterQuality.h"
#include "dialog_select.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(dialog_edit_management, wxDialog)
	EVT_BUTTON(BUTTON_PARAMETER_2, dialog_edit_management::OnParameter2)
	EVT_BUTTON(BUTTON_PARAMETER_6, dialog_edit_management::OnParameter6)
	EVT_COMBOBOX (COMBO_TYPE, dialog_edit_management::OnSelchangeType)
END_EVENT_TABLE()

dialog_edit_management::dialog_edit_management
	(wxWindow* pParent,
	SWATManagement *manage,
	swat_controller *p_boss,
	list_field_help_swat *p_help,
	odbc_database *p_db)
	: wxDialog(pParent, wxID_ANY, "Management")
{
	op = manage;
	boss = p_boss;
	help_list = p_help;
	database = p_db;

    wxBoxSizer *stack = new wxBoxSizer (wxVERTICAL);

	wxBoxSizer *row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, L"Year:"));
	edit_year = new integer_edit (this, wxID_ANY, L"");
	row->Add (edit_year, 1, wxGROW | wxALL, 5);
	row->Add (new wxStaticText (this, wxID_ANY, L"Month:"));
	edit_month = new integer_edit (this, wxID_ANY, L"");
	row->Add (edit_month, 1, wxGROW | wxALL, 5);
	row->Add (new wxStaticText (this, wxID_ANY, L"Day:"));
	edit_day = new integer_edit (this, wxID_ANY, L"");
	row->Add (edit_day, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, L"Heat unit scheduling:"));
	edit_heat_units = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_heat_units, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	{
		int type;
		dynamic_string entry;

		wxArrayString choices;
		for (type = 1; type <= NUMBER_OF_SWAT_OP_CODES; ++type) {
			entry.format ("%d %s", type, swat_operation_name (type).get_text ());
			choices.Add (entry.get_text_ascii ());
		}

		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, L"Operation code number:"));
		combo_type = new wxComboBox (this, COMBO_TYPE, L"", wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
		row->Add (combo_type, 1, wxGROW | wxALL, 5);
		stack->Add (row);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_1 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_1, 1, wxGROW | wxALL, 5);
	edit_parameter_1 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_1, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_2 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_2, 1, wxGROW | wxALL, 5);
	edit_parameter_2 = new integer_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_2, 1, wxGROW | wxALL, 5);
	button_parameter_2 = new wxButton (this, BUTTON_PARAMETER_2, L"Edit");
	row->Add (button_parameter_2);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_3 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_3, 1, wxGROW | wxALL, 5);
	edit_parameter_3 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_3, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_4 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_4, 1, wxGROW | wxALL, 5);
	edit_parameter_4 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_4, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_5 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_5, 1, wxGROW | wxALL, 5);
	edit_parameter_5 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_5, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_6 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_6, 1, wxGROW | wxALL, 5);
	edit_parameter_6 = new integer_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_6, 1, wxGROW | wxALL, 5);
	button_parameter_6 = new wxButton (this, BUTTON_PARAMETER_6, L"Edit");
	row->Add (button_parameter_6);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_7 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_7, 1, wxGROW | wxALL, 5);
	edit_parameter_7 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_7, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_8 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_8, 1, wxGROW | wxALL, 5);
	edit_parameter_8 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_8, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	row = new wxBoxSizer (wxHORIZONTAL);
	static_mgt_9 = new wxStaticText (this, wxID_ANY, L"");
	row->Add (static_mgt_9, 1, wxGROW | wxALL, 5);
	edit_parameter_9 = new float_edit (this, wxID_ANY, L"");
	row->Add (edit_parameter_9, 1, wxGROW | wxALL, 5);
	stack->Add (row);

	stack->Add (CreateButtonSizer (wxOK | wxCANCEL));

	SetSizerAndFit(stack);

	edit_year->Set (op->Year);
	edit_month->Set (op->Month);
	edit_day->Set (op->Day);

	combo_type->SetSelection (op->OperationCode - 1);

	wxCommandEvent ev;
	OnSelchangeType (ev);

	edit_heat_units->Set (op->HeatUnits);
	edit_parameter_1->Set (op->Parameter1);
	edit_parameter_2->Set (op->Parameter2);
	edit_parameter_3->Set (op->Parameter3);
	edit_parameter_4->Set (op->Parameter4);
	edit_parameter_5->Set (op->Parameter5);
	edit_parameter_6->Set (op->Parameter6);
	edit_parameter_7->Set (op->Parameter7);
	edit_parameter_8->Set (op->Parameter8);
	edit_parameter_9->Set (op->Parameter9);
}

void dialog_edit_management::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {

		op->Year = edit_year->Get ();
		op->Month = edit_month->Get ();
		op->Day = edit_day->Get ();
		op->OperationCode = combo_type->GetSelection () + 1;
		op->HeatUnits = edit_heat_units->Get ();
		op->Parameter1 = edit_parameter_1->Get ();
		op->Parameter2 = edit_parameter_2->Get ();
		op->Parameter3 = edit_parameter_3->Get ();
		op->Parameter4 = edit_parameter_4->Get ();
		op->Parameter5 = edit_parameter_5->Get ();
		op->Parameter6 = edit_parameter_6->Get ();
		op->Parameter7 = edit_parameter_7->Get ();
		op->Parameter8 = edit_parameter_8->Get ();
		op->Parameter9 = edit_parameter_9->Get ();
	}
	wxDialog::EndModal (retCode);
}

void dialog_edit_management::describe_parameters ()

{
	SWATTillage *implement;
	SWATCrop *crop;

	button_parameter_2->SetLabel ("??");
	button_parameter_6->SetLabel ("??");
	switch (op->OperationCode) {
		case SWAT_OP_CODE_PLANT:
			if ((crop = boss->crops->match (op->Parameter2)) != NULL)
				button_parameter_2->SetLabel (crop->Name);
			static_mgt_1->SetLabel ("PHU:");
			static_mgt_2->SetLabel ("Crop ID:");
			static_mgt_3->SetLabel ("Harvest Index Target ((kg/ha)/(kg/ha)):");
			static_mgt_4->SetLabel ("DMTAR:");
			static_mgt_5->SetLabel ("Initial Leaf Area Index:");
			static_mgt_6->SetLabel ("");
			static_mgt_7->SetLabel ("DMINIT:");
			static_mgt_8->SetLabel ("SCS Curve No, II:");
			static_mgt_9->SetLabel ("");
			break;
		case SWAT_OP_CODE_IRRIGATE:
			static_mgt_1->SetLabel ("Depth (mm):");
			static_mgt_4->SetLabel ("irr_amt");
			static_mgt_5->SetLabel ("irr_salt");
			if (boss->swat_version >= SWAT_VERSION_2009) {
				static_mgt_6->SetLabel ("irr_efm");
				static_mgt_7->SetLabel ("irr_sq");
			}
			break;
		case SWAT_OP_CODE_HARVEST_AND_KILL:
			static_mgt_1->SetLabel ("");
			static_mgt_2->SetLabel ("");
			static_mgt_3->SetLabel ("");
			static_mgt_4->SetLabel ("");
			if (boss->swat_version >= SWAT_VERSION_2009)
				static_mgt_5->SetLabel ("Fraction stover removed:"); // SWAT2009 Received 2010-08-31
			else
				static_mgt_5->SetLabel (""); // SWAT2009 Received 2010-08-31
			static_mgt_6->SetLabel ("");
			static_mgt_7->SetLabel ("");
			static_mgt_8->SetLabel ("");
			static_mgt_9->SetLabel ("");
			break;
		case SWAT_OP_CODE_HARVEST:
			static_mgt_1->SetLabel ("HI_OVR");
			static_mgt_2->SetLabel ("IHV_GBM");
			static_mgt_3->SetLabel ("HARVEFF");
			static_mgt_4->SetLabel ("");
			static_mgt_5->SetLabel ("");
			static_mgt_6->SetLabel ("");
			static_mgt_7->SetLabel ("");
			static_mgt_8->SetLabel ("SCS Curve No, II:");
			static_mgt_9->SetLabel ("");
			break;
		case SWAT_OP_CODE_TILLAGE:
			if ((implement = boss->tillages->match (op->Parameter2)) != NULL)
				button_parameter_2->SetLabel (implement->Name);
			static_mgt_1->SetLabel ("Parameter 1:");
			static_mgt_2->SetLabel ("Operation ID:");
			static_mgt_3->SetLabel ("");
			static_mgt_4->SetLabel ("");
			static_mgt_5->SetLabel ("");
			static_mgt_6->SetLabel ("");
			static_mgt_7->SetLabel ("");
			static_mgt_8->SetLabel ("SCS Curve No, II:");
			static_mgt_9->SetLabel ("");
			break;
		case SWAT_OP_CODE_KILL:
			static_mgt_1->SetLabel ("");
			static_mgt_2->SetLabel ("");
			static_mgt_3->SetLabel ("");
			static_mgt_4->SetLabel ("");
			static_mgt_5->SetLabel ("");
			static_mgt_6->SetLabel ("");
			static_mgt_7->SetLabel ("");
			static_mgt_8->SetLabel ("");
			static_mgt_9->SetLabel ("");
			break;
		case SWAT_OP_CODE_FERTILIZE:
			{
				SWATFertilizer *fert;
				if ((fert = boss->fertilizers->match (op->Parameter2)) != NULL)
					button_parameter_2->SetLabel (fert->Name);
				static_mgt_1->SetLabel ("Fraction top 10mm:") ;
				static_mgt_2->SetLabel ("Fertilizer ID:");
				static_mgt_3->SetLabel ("Amount applied (kg/ha):");
				static_mgt_4->SetLabel ("");
				static_mgt_5->SetLabel ("");
				static_mgt_6->SetLabel ("");
				static_mgt_7->SetLabel ("");
				static_mgt_8->SetLabel ("");
				static_mgt_9->SetLabel ("");
			}
			break;
		case SWAT_OP_CODE_PESTICIDE:
			{
				SWATPesticide *pest;
				if ((pest = boss->pesticides->match (op->Parameter2)) != NULL)
					button_parameter_2->SetLabel (pest->Name);
				if (boss->swat_version < SWAT_VERSION_2005) {
					static_mgt_1->SetLabel ("") ;
					static_mgt_2->SetLabel ("Pesticide ID:");
					static_mgt_3->SetLabel ("Amount applied (kg/ha):"); // pst_kg
					static_mgt_3->SetLabel ("");
					static_mgt_4->SetLabel ("");
					static_mgt_5->SetLabel ("");
					static_mgt_6->SetLabel ("");
				}
				else {
					static_mgt_1->SetLabel ("ipest") ;
					static_mgt_2->SetLabel ("Pesticide ID:");
					static_mgt_1->SetLabel ("ipest") ;
					static_mgt_2->SetLabel ("Pesticide ID:");
					static_mgt_3->SetLabel ("Amount applied (kg/ha):"); // pst_kg
					if (boss->swat_version >= SWAT_VERSION_2008)
						static_mgt_4->SetLabel ("Depth applied (mm):"); // pest_dep Fortran comment says this was added 3/31/08
					else
						static_mgt_4->SetLabel ("");
					static_mgt_5->SetLabel ("");
					static_mgt_6->SetLabel ("");
				}
				static_mgt_7->SetLabel ("");
				static_mgt_8->SetLabel ("");
				static_mgt_9->SetLabel ("");
			}
			break;
		case SWAT_OP_CODE_GRAZE:
			{
				SWATFertilizer *fert;
				if ((fert = boss->fertilizers->match (op->Parameter6)) != NULL)
					button_parameter_6->SetLabel (fert->Name);
				static_mgt_1->SetLabel ("Consumed ((kg/ha)/day):") ;
				static_mgt_2->SetLabel ("Grazing (days):");
				static_mgt_3->SetLabel ("Trampled ((kg/ha)/day):");
				static_mgt_4->SetLabel ("");
				static_mgt_5->SetLabel ("Manure ((kg/ha)/day):");
				static_mgt_6->SetLabel ("Fertilizer ID:");
				static_mgt_7->SetLabel ("");
				static_mgt_8->SetLabel ("");
				static_mgt_9->SetLabel ("");
			}
			break;
		case SWAT_OP_CODE_INIT_AUTO_IRR:
			static_mgt_2->SetLabel ("WSTRS_ID:");
			static_mgt_3->SetLabel ("AUTO_WSTRS:");
			break;
		case SWAT_OP_CODE_INIT_AUTO_FERT:
			{
				SWATFertilizer *fert;
				if ((fert = boss->fertilizers->match (op->Parameter2)) != NULL)
					button_parameter_2->SetLabel (fert->Name);
				static_mgt_1->SetLabel ("AUTO_NSTRS:");
				static_mgt_2->SetLabel ("Fertilizer ID:");
				static_mgt_3->SetLabel ("AUTO_NAPP:");
				static_mgt_4->SetLabel ("AUTO_NYR:");
				static_mgt_7->SetLabel ("AUTO_EFF:");
				static_mgt_8->SetLabel ("AFRT_SURFACE:");
			}
			break;
		default:
			static_mgt_1->SetLabel ("Parameter 1:");
			static_mgt_2->SetLabel ("Parameter 2:");
			static_mgt_3->SetLabel ("Parameter 3:");
			static_mgt_4->SetLabel ("Parameter 4:");
			static_mgt_5->SetLabel ("Parameter 5:");
			static_mgt_6->SetLabel ("Parameter 6:");
			static_mgt_7->SetLabel ("Parameter 7:");
			static_mgt_8->SetLabel ("Parameter 8:");
			static_mgt_9->SetLabel ("Parameter 9:");
	}
}

void dialog_edit_management::OnParameter2
	(wxCommandEvent &) 
{
	switch (op->OperationCode) {
		case SWAT_OP_CODE_PLANT:
			{
				SWATCrop *crop;

				crop = boss->crops->match (op->Parameter2);

				dialog_select d (this, (void **) &crop, (std::vector <class T> *) &boss->crops->crops, op->OperationCode, false, help_list, database);
				if (d.ShowModal () == wxID_OK) {
					op->Parameter2 = crop->ID;
					edit_parameter_2->Set (op->Parameter2);
					describe_parameters ();
				}
			}
			break;
		case SWAT_OP_CODE_TILLAGE:
		case SWAT_OP_CODE_HARVEST:
		case SWAT_OP_CODE_KILL:
			{
				SWATTillage *till;

				till = boss->tillages->match (op->Parameter2);

				dialog_select d (this, (void **) &till, (std::vector <class T> *) &boss->tillages->tillages, op->OperationCode, false, help_list, database);
				if (d.ShowModal () == wxID_OK) {
					op->Parameter2 = till->ID;
					edit_parameter_2->Set (op->Parameter2);
					describe_parameters ();
				}
			}
			break;
		case SWAT_OP_CODE_FERTILIZE:
		case SWAT_OP_CODE_INIT_AUTO_FERT:
			{
				SWATFertilizer *till;

				till = boss->fertilizers->match (op->Parameter2);

				dialog_select d (this, (void **) &till, (std::vector <class T> *) &boss->fertilizers->fertilizers, SWAT_OP_CODE_FERTILIZE, false, help_list, database);
				if (d.ShowModal () == wxID_OK) {
					op->Parameter2 = till->ID;
					edit_parameter_2->Set (op->Parameter2);
					describe_parameters ();
				}
			}
			break;
		case SWAT_OP_CODE_GRAZE:
			{
				SWATFertilizer *till;

				till = boss->fertilizers->match (op->Parameter6);

				dialog_select d (this, (void **) &till, (std::vector <class T> *) &boss->fertilizers->fertilizers, SWAT_OP_CODE_FERTILIZE, false, help_list, database);
				if (d.ShowModal () == wxID_OK) {
					op->Parameter6 = (short) till->ID;
					edit_parameter_6->Set (op->Parameter6);
					describe_parameters ();
				}
			}
			break;
		case SWAT_OP_CODE_PESTICIDE:
			{
				SWATPesticide *till;

				till = boss->pesticides->match (op->Parameter2);

				dialog_select d (this, (void **) &till, (std::vector <class T> *) &boss->pesticides->pesticides, SWAT_OP_CODE_PESTICIDE, false, help_list, database);
				if (d.ShowModal () == wxID_OK) {
					op->Parameter2 = till->ID;
					edit_parameter_2->Set (op->Parameter2);
					describe_parameters ();
				}
			}
	}
}

void dialog_edit_management::OnParameter6
	(wxCommandEvent &) 
{
	if (op->OperationCode == SWAT_OP_CODE_GRAZE) {
		SWATFertilizer *till;

		till = boss->fertilizers->match (op->Parameter6);

		dialog_select d (this, (void **) &till, (std::vector <class T> *) &boss->fertilizers->fertilizers, SWAT_OP_CODE_FERTILIZE, false, help_list, database);
		if (d.ShowModal () == wxID_OK) {
			op->Parameter6 = (short) till->ID;
			edit_parameter_6->Set (op->Parameter6);
			describe_parameters ();
		}
	}
}

void dialog_edit_management::OnSelchangeType
	(wxCommandEvent &) 
{
	op->OperationCode = combo_type->GetSelection () + 1;

	switch (op->OperationCode) {
		case SWAT_OP_CODE_PLANT:
		case SWAT_OP_CODE_TILLAGE:
		case SWAT_OP_CODE_HARVEST:
		case SWAT_OP_CODE_KILL:
		case SWAT_OP_CODE_FERTILIZE:
		case SWAT_OP_CODE_INIT_AUTO_FERT:
			button_parameter_2->Enable (true);
			button_parameter_6->Enable (false);
			break;
		case SWAT_OP_CODE_GRAZE:
			button_parameter_2->Enable (false);
			button_parameter_6->Enable (true);
			break;
		default:
			button_parameter_2->Enable (false);
			button_parameter_6->Enable (false);
	}
	describe_parameters ();	
}
