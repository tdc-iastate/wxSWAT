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
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/custom.h"
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
#include "page_hru_2.h"
#include "../weather/averager_weighted.h"
#include "../i_epic_database/EPICDeclarations.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(page_hru_2, wxNotebookPage)
	EVT_BUTTON (BUTTON_SET_URBAN_LAND_TYPE, page_hru_2::OnSetUrbanLandType)
	EVT_COMBOBOX (COMBO_IRRIGATION_CODE, page_hru_2::OnSelchangeIrrigationCode)
	EVT_CHILD_FOCUS (page_hru_2::OnChildFocus)
END_EVENT_TABLE()

page_hru_2::page_hru_2
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
	wxBoxSizer *stack, *row, *column;

	hru = phru;
	subbasin = pbasin;
	control = pcontrol;
	controller = pcontroller;
	help = p_help;
	database = p_db;

	stack = new wxBoxSizer( wxVERTICAL );


	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Initial residue cover, kg/ha:"));
	row->AddSpacer (10);
	edit_initial_residue_cover = new float_edit (this, EDIT_INITIAL_RESIDUE_COVER, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_initial_residue_cover);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Organic N enrichment ratio:"));
	row->AddSpacer(10);
	edit_organic_n_enrichment_ratio = new float_edit (this, EDIT_ORGANIC_N_ENRICHMENT_RATIO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_organic_n_enrichment_ratio);
	row->AddSpacer (15);
	row->Add (new wxStaticText (this, wxID_ANY, "Organic P enrichment ratio:"));
	row->AddSpacer(10);
	edit_organic_p_enrichment_ratio = new float_edit (this, EDIT_ORGANIC_P_ENRICHMENT_RATIO, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_organic_p_enrichment_ratio);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Width of edge-of-field filter strip, m:"));
	row->AddSpacer(10);
	edit_width_of_edge_of_field_filter_strip = new float_edit (this, EDIT_WIDTH_OF_EDGE_OF_FIELD_FILTER_STRIP, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_width_of_edge_of_field_filter_strip);
	stack->Add (row, 0, wxALL, 5);

	wxStaticBoxSizer* outline_box = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("Urban Land:"));

	column = new wxBoxSizer (wxVERTICAL);

	row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(new wxStaticText(this, wxID_ANY, "Urban simulation code:"));
	row->AddSpacer (10);
	wxString items[3];
	items[0] = "0 No urban sections";
	items[1] = "1 USGS regression equations";
	items[2] = "2 Build up/wash off algorithm";
	combo_urban_simulation_code = new wxComboBox(this, COMBO_URBAN_SIMULATION_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 3, items, wxCB_DROPDOWN);
	row->Add(combo_urban_simulation_code);
	column->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Urban land type:"));
	static_urban_land_type_id = new wxStaticText (this, STATIC_URBAN_LAND_TYPE_ID, wxT("ID"));
	row->Add (static_urban_land_type_id);
	row->AddSpacer (10);
	row->Add (new wxButton (this, BUTTON_SET_URBAN_LAND_TYPE, wxT("Set")), 0, wxALL, 5);
	column->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(new wxStaticText(this, wxID_ANY, "Urban BMP runoff sediment:"));
	edit_sed_con = new float_edit(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add(edit_sed_con);
	column->Add(row, 0, wxALL, 5);

	row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(new wxStaticText(this, wxID_ANY, "Urban BMP runoff organic P:"));
	edit_orgp_con = new float_edit(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add(edit_orgp_con);
	column->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(new wxStaticText(this, wxID_ANY, "Urban BMP runoff soluble N:"));
	edit_soln_con = new float_edit(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add(edit_soln_con);
	column->Add(row, 0, wxALL, 5);

	outline_box->Add (column);

	column = new wxBoxSizer(wxVERTICAL);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Urban BMP runoff organic N:"));
	edit_orgn_con = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_orgn_con);
	column->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer(wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Urban BMP runoff soluble P:"));
	edit_solp_con = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_solp_con);
	column->Add (row, 0, wxALL, 5);

	outline_box->Add(column);
	stack->Add (outline_box, 0, wxALL, 5);

	{
		row = new wxBoxSizer (wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, "Irrigation code:"));
		row->AddSpacer (10);
		wxString items [6];
		items [0] = "0 No irrigation";
		items [1] = "1 Reach";
		items [2] = "2 Reservoir";
		items [3] = "3 Shallow aquifer";
		items [4] = "4 Deep aquifer";
		items [5] = "5 Outside/unlimited";
		combo_irrigation_code = new wxComboBox (this, COMBO_IRRIGATION_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 6, items, wxCB_DROPDOWN);
		row->Add (combo_irrigation_code);
		stack->Add (row, 0, wxALL, 5);
	}

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add(new wxStaticText(this, wxID_ANY, "Irrigation source location:"));
	row->AddSpacer (10);
	static_irrigation_source_location = new integer_static (this, STATIC_IRRIGATION_SOURCE_LOCATION, wxT("Source"));
	row->Add (static_irrigation_source_location);
	row->AddSpacer(10);
	edit_irrigation_source_location = new integer_edit (this, EDIT_IRRIGATION_SOURCE_LOCATION, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_irrigation_source_location);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Minimum in-stream flow for irrigation diversions, m^3/s:"));
	row->AddSpacer(10);
	edit_minimum_instream_flow = new float_edit (this, EDIT_MINIMUM_INSTREAM_FLOW, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_minimum_instream_flow);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Maximum daily irrigation diversion:"));
	row->AddSpacer(10);
	edit_maximum_daily_irrigation_diversion = new float_edit (this, EDIT_MAXIMUM_DAILY_IRRIGATION_DIVERSION, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_maximum_daily_irrigation_diversion);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer (wxHORIZONTAL);
	row->Add (new wxStaticText (this, wxID_ANY, "Fraction of available flow:"));
	row->AddSpacer(10);
	edit_fraction_of_available_flow = new float_edit (this, EDIT_FRACTION_OF_AVAILABLE_FLOW, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_fraction_of_available_flow);
	stack->Add (row, 0, wxALL, 5);

	edit_help = new wxTextCtrl (this, EDIT_HELP_2, wxEmptyString, wxDefaultPosition, wxSize (550, 100), wxTE_MULTILINE | wxTE_WORDWRAP);
	stack->Add (edit_help);
	
	SetSizerAndFit(stack);

	// button_water_use.EnableWindow (basin->water_use != NULL);
}

bool page_hru_2::TransferDataToWindow ()

{
	SWATUrban *urban;

	edit_initial_residue_cover->Set (hru->InitialResidueCover);
	edit_organic_n_enrichment_ratio->Set (hru->OrganicNEnrichmentRatio);
	edit_organic_p_enrichment_ratio->Set (hru->OrganicPEnrichmentRatio);
	edit_width_of_edge_of_field_filter_strip->Set (hru->FilterStripWidth);
	combo_urban_simulation_code->SetSelection (hru->UrbanSimulationCode - 1);

	if ((urban = controller->urbans->match (hru->UrbanLandID)) != NULL)
		static_urban_land_type_id->SetLabelText (urban->Name);
	else
		static_urban_land_type_id->SetLabelText ("");

	edit_sed_con->Set (hru->UrbanBMPRunoffSediment);
	edit_orgn_con->Set (hru->UrbanBMPRunoffOrganicN);
	edit_orgp_con->Set (hru->UrbanBMPRunoffOrganicP);
	edit_soln_con->Set (hru->UrbanBMPRunoffSolubleN);
	edit_solp_con->Set (hru->UrbanBMPRunoffSolubleP);

	static_irrigation_source_location->Set (hru->IrrigationSourceLocation);
	edit_minimum_instream_flow->Set (hru->StreamFlowIrrigationMinimum);
	edit_maximum_daily_irrigation_diversion->Set (hru->IrrigationDailyMaximum);
	edit_fraction_of_available_flow->Set (hru->FractionofAvailableFlow);

	combo_irrigation_code->SetSelection (hru->IrrigationCode);
	show_irrigation_source ();
	edit_irrigation_source_location->Set (hru->IrrigationSourceLocation);

	return true;
}

bool page_hru_2::TransferDataFromWindow ()

{
	hru->InitialResidueCover = edit_initial_residue_cover->Get ();
	hru->OrganicNEnrichmentRatio = edit_organic_n_enrichment_ratio->Get ();
	hru->OrganicPEnrichmentRatio = edit_organic_p_enrichment_ratio->Get ();
	hru->FilterStripWidth = edit_width_of_edge_of_field_filter_strip->Get ();
	hru->UrbanSimulationCode = combo_urban_simulation_code->GetSelection () + 1;
	hru->UrbanBMPRunoffSediment = edit_sed_con->Get ();
	hru->UrbanBMPRunoffOrganicN = edit_orgn_con->Get ();
	hru->UrbanBMPRunoffOrganicP = edit_orgp_con->Get ();
	hru->UrbanBMPRunoffSolubleN = edit_soln_con->Get ();
	hru->UrbanBMPRunoffSolubleP = edit_solp_con->Get ();

	hru->IrrigationCode = combo_irrigation_code->GetSelection () + 1;
	hru->IrrigationSourceLocation = edit_irrigation_source_location->Get ();
	hru->StreamFlowIrrigationMinimum = edit_minimum_instream_flow->Get ();
	hru->IrrigationDailyMaximum = edit_maximum_daily_irrigation_diversion->Get ();
	hru->FractionofAvailableFlow = edit_fraction_of_available_flow->Get ();

	return true;
}

void page_hru_2::show_irrigation_source ()

{
	switch (combo_irrigation_code->GetSelection ()) {
		case 0: // no irrigation
		case 5: // Outside
			static_irrigation_source_location->SetLabelText ("");
			edit_irrigation_source_location->Disable ();
			break;
		case 1: // Reach
			static_irrigation_source_location->SetLabelText ("Reach");
			edit_irrigation_source_location->Enable ();
			break;
		case 2: // Reservoir
			static_irrigation_source_location->SetLabelText ("Reservoir");
			edit_irrigation_source_location->Enable ();
			break;
		case 3: // SubBasin
		case 4: // SubBasin
			static_irrigation_source_location->SetLabelText ("Sub-basin");
			edit_irrigation_source_location->Enable ();
	}
}

void page_hru_2::OnSelchangeIrrigationCode (wxCommandEvent &)

{
	show_irrigation_source ();
}


void page_hru_2::OnSetUrbanLandType
	(wxCommandEvent &)
{
	SWATUrban *urban;

	urban = controller->urbans->match (hru->UrbanLandID);

	/*
	dialog_select_urban d (this, urbans, &urban, help_list);
	if (d.DoModal () == IDOK) {
		hru->UrbanLandID = urban->ID;
		static_urban_land_type_id->SetLabelText (urban->Name);
	}
	*/
}

void page_hru_2::OnChildFocus (wxChildFocusEvent &)

{
	dynamic_string help_text;
	switch (FindFocus ()->GetId ()) {
		case EDIT_INITIAL_RESIDUE_COVER:
			help->get_help ("RSDIN", controller->swat_version, help_text);
			break;
		case EDIT_ORGANIC_N_ENRICHMENT_RATIO:
			help->get_help ("EORGN", controller->swat_version, help_text);
			break;
		case EDIT_ORGANIC_P_ENRICHMENT_RATIO:
			help->get_help ("EORGP", controller->swat_version, help_text);
			break;
		case EDIT_WIDTH_OF_EDGE_OF_FIELD_FILTER_STRIP:
			help->get_help ("FILTERW", controller->swat_version, help_text);
			break;
		case COMBO_URBAN_SIMULATION_CODE:
			help->get_help ("IURBAN", controller->swat_version, help_text);
			break;
		case COMBO_IRRIGATION_CODE:
			help->get_help ("IRR", controller->swat_version, help_text);
			break;
		case EDIT_IRRIGATION_SOURCE_LOCATION:
			help->get_help ("IRRNO", controller->swat_version, help_text);
			break;
		case EDIT_MINIMUM_INSTREAM_FLOW:
			help->get_help ("FLOWMIN", controller->swat_version, help_text);
			break;
		case EDIT_MAXIMUM_DAILY_IRRIGATION_DIVERSION:
			help->get_help ("DIVMAX", controller->swat_version, help_text);
			break;
		case EDIT_FRACTION_OF_AVAILABLE_FLOW:
			help->get_help ("FLOWFR", controller->swat_version, help_text);
	}
	if (help_text.get_length () > 0)
		edit_help->SetValue (help_text.get_text ());
}