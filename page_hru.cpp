#include "../util/utility_afx.h"
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/msgdlg.h>
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/custom.h"
#include "../util/dialog_error_list.h"
#include "../util/interface_window.h"
#include "../util/ARMS_Declarations.h"
#include "../util/list_field_help.h"
#include "../util/device_coordinate.h"

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"

// #include "propsheet_control_record.h"
// #include "bitmaps/wizard_control_record.xpm"
#include "../i_swat_database/SWATDeclarations.h"
#include "../i_swat_database/SWATSoilLayers.h"
#include "../i_swat_database/SWATSoils.h"
#include "../i_swat_database/SWATTillage.h"
#include "../i_swat_database/SWATUrban.h"
#include "../i_swat_database/SWATFertilizer.h"
#include "../i_swat_database/SWATPesticide.h"
#include "../i_swat_database/SWATCrop.h"
#include "../i_swat_database/SWATRouting.h"
#include "../i_swat_database/SWATPointSources.h"
#include "../i_swat_database/SWATPointSourcesMonthly.h"
#include "../i_swat_database/SWATPointSourcesDaily.h"
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
#include "../i_swat_database/SWATManagement.h"
#include "../i_swat_database/SWATTemporalConservationPractices.h"
#include "../i_swat_database/rowset_hru.h"
#include "../i_swat_database/SWATWeatherHistorical.h"
#include "../i_swat_database/SWATWeatherByMonth.h"
#include "../i_swat_database/SWATWeatherStations.h"
#include "../i_swat_database/swat_weather_list.h"
#include "../i_swat_database/rowset_subbasin.h"
#include "../i_swat_database/SWATPotentialEvapotranspiration.h"
#include "../i_swat_database/SWATControlRecord.h"
#include "../i_swat_database/swat_controller.h"
#include "../i_swat_database/SWATRetentionIrrigationBasin.h"
#include "../i_swat_database/SWATSedimentationFiltrationBasin.h"
#include "../i_swat_database/SWATStreamWaterQuality.h"
#include "../i_swat_database/SWATPond.h"
#include "page_hru.h"
#include "../weather/averager_weighted.h"
#include "../i_epic_database/EPICDeclarations.h"
#include "dialog_edit_management.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(page_hru, wxPanel)
	EVT_BUTTON (BUTTON_ADD_OPERATION, page_hru::OnAddOperation)
	EVT_BUTTON (BUTTON_EDIT_OPERATION, page_hru::OnEditOperation)
	EVT_BUTTON (BUTTON_DELETE_OPERATION, page_hru::OnDeleteOperation)
	EVT_BUTTON (BUTTON_LIST_OPS, page_hru::OnListOps)
	EVT_CHILD_FOCUS (page_hru::OnChildFocus)
	EVT_LISTBOX_DCLICK (LIST_OPERATIONS, page_hru::OnEditOperation)
END_EVENT_TABLE()

page_hru::page_hru
	(wxWindow *parent,
	const int id,
	HRU_record *phru,
	SubBasin *pbasin,
	SWATControlRecord *pcontrol,
	swat_controller *pcontroller,
	list_field_help_swat *p_help,
	class odbc_database *p_db)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row, *column, *small_row;

	hru = phru;
	subbasin = pbasin;
	control = pcontrol;
	controller = pcontroller;
	database = p_db;
	help = p_help;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer (wxHORIZONTAL);
	edit_hru = new long_integer_edit (this, EDIT_HRU, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_hru);
	row->AddStretchSpacer ();
	edit_description = new wxTextCtrl (this, EDIT_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize (320, 24));
	row->Add (edit_description);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 2
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Rotation:")));
	{
		wxString items [NUMBER_OF_ARMS_ROTATIONS];

		for (int rotation_index = 0; rotation_index < NUMBER_OF_ARMS_ROTATIONS; ++rotation_index)
			items [rotation_index] = arms_rotation_name (rotation_index + 1).get_text_wide ();

		combo_rotation = new wxComboBox (this, COMBO_ROTATION, wxEmptyString, wxDefaultPosition, wxDefaultSize, NUMBER_OF_ARMS_ROTATIONS, items, wxCB_DROPDOWN);
		row->Add (combo_rotation);
	}
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Rotation length:")));
	edit_rotation_length = new integer_edit (this, EDIT_ROTATION_LENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_rotation_length);
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Runoff curve number:")));
	edit_curve_number = new float_edit (this, EDIT_CURVE_NUMBER, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_curve_number);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 3
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Tillage:")));
	row->AddStretchSpacer ();
	wxString items [NUMBER_OF_TILLAGES_EPIC];
	for (int tillage_index = 0; tillage_index < NUMBER_OF_TILLAGES_EPIC; ++tillage_index) 
		items [tillage_index] = epic_tillage_name (tillage_index + 1).get_text_wide ();
	combo_tillage = new wxComboBox (this, COMBO_TILLAGE, wxEmptyString, wxDefaultPosition, wxDefaultSize, NUMBER_OF_TILLAGES_EPIC, items, wxCB_DROPDOWN);
	row->Add (combo_tillage);
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Location code:")));
	row->AddStretchSpacer ();
	edit_location_code = new long_integer_edit (this, EDIT_LOCATION_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_location_code);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 4
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Soil:")));
	row->AddStretchSpacer (1);
	static_soil = new wxStaticText (this, STATIC_SOIL, wxT("Soil"));
	row->Add (static_soil, 3);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 5
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Slope:")));
	row->AddStretchSpacer ();
	edit_slope = new float_edit (this, EDIT_SLOPE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_slope);
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Slope length, m:")));
	row->AddStretchSpacer ();
	edit_slope_length = new float_edit (this, EDIT_SLOPE_LENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_slope_length);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 6
	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Mannings n overland:")));
	row->AddStretchSpacer ();
	edit_mannings_n_coefficient = new float_edit (this, EDIT_MANNINGS_N_COEFFICIENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_mannings_n_coefficient);
	row->AddStretchSpacer ();
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Lateral flow travel time, days:")));
	row->AddStretchSpacer ();
	edit_lateral_flow_travel_time = new float_edit (this, EDIT_LATERAL_FLOW_TRAVEL_TIME, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_lateral_flow_travel_time);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Area, km^2:")), 0, wxALL, 5);
	row->AddSpacer (5);
	edit_fraction_of_total_watershed_area = new double_edit (this, EDIT_FRACTION_OF_TOTAL_WATERSHED_AREA, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_fraction_of_total_watershed_area, 0, wxALL, 5);

	row->AddSpacer(15);
	row->Add(new wxStaticText(this, wxID_ANY, wxT("Fraction of subbasin:")), 0, wxALL, 5);
	row->AddSpacer(5);
	static_fractionofsubbasin = new double_static(this, STATIC_FRACTIONOFSUBBASIN, wxT(""));
	row->Add(static_fractionofsubbasin, 0, wxALL, 5);

	row->AddSpacer (15);
	row->Add(new wxStaticText(this, wxID_ANY, wxT("Fraction of basin:")), 0, wxALL, 5);
	row->AddSpacer(5);
	static_fractionofwatershed = new double_static(this, STATIC_FRACTIONOFWATERSHED, wxT(""));
	row->Add(static_fractionofwatershed, 0, wxALL, 5);

	stack->Add (row, 0, wxALL, 5);

	{
		wxStaticBoxSizer* outline_box = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("Operations:"));
		column = new wxBoxSizer (wxVERTICAL);
		column->Add (new wxButton (this, BUTTON_ADD_OPERATION, wxT("Add")));
		column->Add (new wxButton (this, BUTTON_EDIT_OPERATION, wxT("Edit")));
		column->Add (new wxButton (this, BUTTON_DELETE_OPERATION, wxT("Delete")));
		column->Add (new wxButton (this, BUTTON_LIST_OPS, wxT("List")));
		outline_box->Add (column, 0, wxALL, 5);
		outline_box->AddSpacer (15);
		list_operations = new wxListBox (this, LIST_OPERATIONS, wxDefaultPosition, wxSize (400, 110));
		outline_box->Add (list_operations, 0, wxALL, 5);
		stack->Add(outline_box, 0, wxALL, 5);
	}

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxSize(550, 100), wxTE_MULTILINE | wxTE_WORDWRAP);
	stack->Add (edit_help, 0, wxALL, 5);
	
	SetSizerAndFit(stack);

	// button_water_use.EnableWindow (basin->water_use != NULL);

	// set_controls ();
}

bool page_hru::TransferDataToWindow ()

{
	edit_hru->Set (hru->HRU);
	edit_description->SetValue (hru->Description);
	combo_rotation->SetSelection (hru->Rotation - 1);
	edit_rotation_length->Set (hru->RotationLength);
	edit_curve_number->Set (hru->RunoffCurveNumber);
	combo_tillage->SetSelection (hru->Tillage - 1);
	edit_location_code->Set (hru->LocationCode);
	edit_slope->Set (hru->Slope);
	edit_slope_length->Set (hru->SlopeLength);
	edit_mannings_n_coefficient->Set (hru->ManningsNValue);
	edit_lateral_flow_travel_time->Set (hru->LateralFlowTravelTime);
	edit_fraction_of_total_watershed_area->Set (hru->Area);

	SWATSoils *soil;
	dynamic_string entry;
	entry.format ("%d", hru->SoilID);
	if ((soil = controller->soils->match (hru->SoilID)) != NULL) {
		entry += " ";
		entry += soil->Name;
	}
	static_soil->SetLabelText (entry.get_text_wide ());
	show_area_fraction ();
	show_management ();
	return true;
}

bool page_hru::TransferDataFromWindow ()

{
	hru->HRU = edit_hru->Get ();
	strcpy_s (hru->Description, SWAT_DESCRIPTION_FIELD_LENGTH, edit_description->GetValue ().ToAscii ());
	hru->Rotation = combo_rotation->GetSelection () + 1;
	hru->RotationLength = edit_rotation_length->Get ();
	hru->RunoffCurveNumber = edit_curve_number->Get ();
	hru->Tillage = combo_tillage->GetSelection () + 1;
	hru->LocationCode = edit_location_code->Get ();
	hru->Slope = edit_slope->Get ();
	hru->SlopeLength = edit_slope_length->Get ();
	hru->ManningsNValue = edit_mannings_n_coefficient->Get ();
	hru->LateralFlowTravelTime = edit_lateral_flow_travel_time->Get ();
	hru->Area = edit_fraction_of_total_watershed_area->Get ();

	return true;
}

void page_hru::show_area_fraction ()

// Display the absolue area (km^2) and fraction of subbasin
// represented by this HRU's FractionofTotalWatershedArea

{
	double fraction;

	fraction = hru->Area / subbasin->total_area_km2 ();
	static_fractionofsubbasin->Set (fraction);
	fraction = hru->Area / control->total_area_km2 ();
	static_fractionofwatershed->Set (fraction);
}

void page_hru::show_management ()

{
	std::vector <SWATManagement>::const_iterator op;
	SWATTillage *implement;
	SWATFertilizer *fert;
	SWATCrop *crop;
	dynamic_string description;

	list_operations->Clear ();

	for (op = hru->operations.ops.begin ();
	op != hru->operations.ops.end ();
	++op) {
		description.format ("%04d-%02d-%02d %s", op->Year, op->Month, op->Day, swat_operation_name (op->OperationCode).get_text ());

		switch (op->OperationCode) {
			case SWAT_OP_CODE_PLANT:
				description += " ";
				if ((crop = controller->crops->match (op->Parameter2)) != NULL)
					description += crop->Name;
				else
					description.add_formatted (" %d", (int) op->Parameter2);
				break;
			case SWAT_OP_CODE_FERTILIZE:
				description += " ";
				if ((fert = controller->fertilizers->match (op->Parameter2)) != NULL)
					description += fert->Name;
				else
					description.add_formatted (" %d", (int) op->Parameter2);
				break;
			case SWAT_OP_CODE_HARVEST_AND_KILL:
			case SWAT_OP_CODE_HARVEST:
			case SWAT_OP_CODE_TILLAGE:
			case SWAT_OP_CODE_KILL:
				description += " ";
				if ((implement = controller->tillages->match (op->Parameter2)) != NULL)
					description += implement->Name;
				else
					description.add_formatted (" %d", (int) op->Parameter2);
				/*
				break;
			case SWAT_OP_CODE_GRAZE:
				entry.Format ("%04d-%02d-%02d Graze %f %d", op->Year, op->Month, op->Day, op->Parameter1,
				(int) op->Parameter2);
				break;
			case SWAT_OP_CODE_INIT_AUTO_IRR:
				entry.Format ("%04d-%02d-%02d Auto Irr %f %d", op->Year, op->Month, op->Day, op->Parameter1,
				(int) op->Parameter2);
				break;
			case SWAT_OP_CODE_INIT_AUTO_FERT:
				entry.Format ("%04d-%02d-%02d Auto Fert %f %d", op->Year, op->Month, op->Day, op->Parameter1,
				(int) op->Parameter2);
				break;
			case SWAT_OP_CODE_STREET_SWEEP:
				entry.Format ("%04d-%02d-%02d Street Sweep %f %d", op->Year, op->Month, op->Day, op->Parameter1,
				(int) op->Parameter2);
				break;
			case SWAT_OP_CODE_WATER_RELEASE_IMPOUND:
				entry.Format ("%04d-%02d-%02d Water Release Impound %f %d", op->Year, op->Month, op->Day, op->Parameter1,
				(int) op->Parameter2);
				*/
		}

		list_operations->Append (description.get_text_wide ());
	}
}

void page_hru::OnAddOperation (wxCommandEvent &)

{
	int index;
	SWATManagement op;

	if ((index = list_operations->GetSelection ()) != -1)
		op.copy (hru->operations.ops [index]);

	hru->operations.ops.push_back (op);
	show_management ();
}

void page_hru::OnEditOperation (wxCommandEvent &)

{
	int index;
	SWATManagement op;

	if ((index = list_operations->GetSelection ()) != -1) {
		op.copy (hru->operations.ops [index]);
		dialog_edit_management d (this, &op, controller, help, database);
		if (d.ShowModal () == wxID_OK) {
			hru->operations.ops [index].copy (op);
			show_management ();
		}
	}
}

void page_hru::OnDeleteOperation (wxCommandEvent &)

{
	int index;
	SWATManagement *op;
	dynamic_string prompt;

	if ((index = list_operations->GetSelection ()) != -1) {
		op = &hru->operations.ops[index];

		prompt = "Delete ";
		prompt += swat_operation_name (op->OperationCode);
		prompt.add_formatted (" operation on %d/%d?", (int) op->Month, (int) op->Day);

		if (wxMessageBox (prompt.get_text_wide (), "Confirm Delete", wxYES_NO) == wxYES) {
			hru->operations.remove (op);
			show_management ();
		}
	}
}

void page_hru::OnListOps (wxCommandEvent &)

{
	dynamic_string report;
	std::vector <SWATManagement>::const_iterator op;
	SWATTillage *implement;
	SWATFertilizer *fert;
	SWATCrop *crop;
	dynamic_string description;
	int year = 1;

	report.add_formatted ("%ld\t%ld\t", hru->Subbasin, hru->HRU, hru->Description);
	report += hru->Description;
	report += "\n\n";

	for (op = hru->operations.ops.begin ();
	op != hru->operations.ops.end ();
	++op) {
		if (op->Year != year) {
			report += "\n";
			year = op->Year;
		}
		report.add_formatted ("%04d-%02d-%02d\t%.2f\t", op->Year, op->Month, op->Day, op->HeatUnits);
		report += swat_operation_name (op->OperationCode);

		switch (op->OperationCode) {
			case SWAT_OP_CODE_PLANT:
				if ((crop = controller->crops->match (op->Parameter2)) != NULL) {
					report += "\t";
					report += crop->Name;
				}
				else
					report.add_formatted ("\t%d", (int) op->Parameter2);
				report.add_formatted ("\tPHU=%.2f", op->Parameter1);
				report.add_formatted ("\tCN=%.2f", op->Parameter8);
				break;
			case SWAT_OP_CODE_FERTILIZE:
				if ((fert = controller->fertilizers->match (op->Parameter2)) != NULL) {
					report += "\t";
					report += fert->Name;
				}
				else
					report.add_formatted ("\t%d", (int) op->Parameter2);
				report.add_formatted ("\t%.2f kg/ha", op->Parameter3);
				if (op->Parameter1 != 0.0f)
					report.add_formatted ("\t%.2f in top 10mm", op->Parameter1);
				break;
			case SWAT_OP_CODE_HARVEST_AND_KILL:
			case SWAT_OP_CODE_HARVEST:
				/*
				if ((implement = doc->tillages.match (op->Parameter2)) != NULL)
					report.add_formatted ("\t%s", implement->Name);
				else
				*/
				report.add_formatted ("\t%.2f", op->Parameter1);  // Harvest override for stover
				report.add_formatted ("\t%ld", op->Parameter2);
				report.add_formatted ("\t%.2f", op->Parameter3); // HARVEFF
				break;
			case SWAT_OP_CODE_TILLAGE:
			case SWAT_OP_CODE_KILL:
				if ((implement = controller->tillages->match (op->Parameter2)) != NULL) {
					report += "\t";
					report += implement->Name;
				}
				else
					report.add_formatted ("\t%d", (int) op->Parameter2);
		}
		report += "\n";
	}

	report.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Operation List", L"", report);
	d.ShowWindowModal ();
}

void page_hru::OnChildFocus (wxChildFocusEvent &)

{
	dynamic_string help_text;
	switch (FindFocus ()->GetId ()) {
		case EDIT_ROTATION_LENGTH:
				help->get_help ("NROT", controller->swat_version, help_text);
			break;
		case EDIT_CURVE_NUMBER:
			help->get_help ("CN2", controller->swat_version, help_text);
			break;
		case COMBO_TILLAGE:
			break;
		case EDIT_SLOPE:
			help->get_help ("SLOPE", controller->swat_version, help_text);
			break;
		case EDIT_SLOPE_LENGTH:
			help->get_help ("SLSUBBSN", controller->swat_version, help_text);
			break;
		case EDIT_MANNINGS_N_COEFFICIENT:
			help->get_help ("OV_N", controller->swat_version, help_text);
			break;
		case EDIT_LATERAL_FLOW_TRAVEL_TIME:
			help->get_help ("LAT_TTIME", controller->swat_version, help_text);
			break;
	    case EDIT_FRACTION_OF_TOTAL_WATERSHED_AREA:
			help->get_help ("HRU_FR", controller->swat_version, help_text);
	}
	if (help_text.get_length () > 0)
		edit_help->SetValue (help_text.get_text ());
}