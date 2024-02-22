#include "../util/utility_afx.h"
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <wx/frame.h>
#include <wx/bookctrl.h>
#include <wx/propdlg.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/list.h"
#include "../util/list_field_help.h"
#include "../util/custom.h"
#include "../util/dialog_error_list.h"
#include "../util/device_coordinate.h"

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"

#include "propsheet_routing.h"
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
#include "../i_swat_database/control_record_report.h"
#include "page_control_record.h"
#include "propsheet_basin.h"
#include "propsheet_subbasin.h"
#include "list_field_help_swat.h"

page_control_record::page_control_record
	(wxWindow *parent,
	SWATControlRecord *pcontrol,
	swat_controller *pcontroller,
	list_field_help_swat *p_help,
	odbc_database *p_db)
	: wxPanel (parent, wxID_ANY)

{
	wxBoxSizer *stack, *row, *small_row;

	control = pcontrol;
	controller = pcontroller;
	help_list = p_help;
	db = p_db;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Description:")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	row->Add (new wxButton (this, BUTTON_LANDUSE_REPORT, wxT("Landuse")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_description = new wxTextCtrl (this, EDIT_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize (400, 20));
	row->Add (edit_description, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	// stack->Add (row, 1, wxALL);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("ID:")), 0, wxALIGN_BOTTOM);
	row->AddSpacer (10);
	static_id = new integer_static (this, STATIC_ID, wxT("ID"));
	row->Add (static_id, 0, wxALIGN_BOTTOM);
	// stack->Add (row, 1, wxALL);
	stack->Add (row, 0, wxALL, 5);

	// stack->AddSpacer (5);

	// Starting here, two columns down
	row = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *stack_left = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *stack_right= new wxBoxSizer( wxVERTICAL );

	// Row 1 Left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Run length:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	small_row->AddStretchSpacer ();
	edit_run_length = new integer_edit (this, EDIT_RUN_LENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_run_length, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack_left->Add(small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 1 Right
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Print code:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	small_row->AddStretchSpacer ();
	{
		wxString items [3];
		items [0] = "0 Monthly";
		items [1] = "1 Daily";
		items [2] = "2 Yearly";
		combo_print_code = new wxComboBox (this, COMBO_PRINT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 3, items, wxCB_DROPDOWN);
	}
	small_row->Add (combo_print_code, 0, wxALL, 5);
	// stack_right->Add (small_row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	stack_right->Add (small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 2 Left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("First year:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	small_row->AddStretchSpacer ();
	edit_first_year = new integer_edit (this, EDIT_FIRST_YEAR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_first_year, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack_left->Add (small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 2 Right
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Skip years:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	small_row->AddStretchSpacer ();
	edit_skip_years = new integer_edit (this, EDIT_SKIP_YEARS, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_skip_years, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	// stack_right->Add (small_row, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_RIGHT);
	stack_right->Add (small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 3 left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("First julian day:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	small_row->AddStretchSpacer (1);
	edit_first_julian_day = new integer_edit (this, EDIT_FIRST_JULIAN_DAY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_first_julian_day, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack_left->Add (small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 3 right
	check_condense_std_output = new wxCheckBox (this, CHECK_CONDENSE_STD_OUTPUT, wxT("Condense .STD output"), wxDefaultPosition, wxDefaultSize, wxBG_STYLE_TRANSPARENT);
	stack_right->Add (check_condense_std_output, 0, wxALL, 5);

	// Row 4 left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Last julian day:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	small_row->AddStretchSpacer (1);
	edit_last_julian_day = new integer_edit (this, EDIT_LAST_JULIAN_DAY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_last_julian_day, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack_left->Add (small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 4 right
	check_streamflow_logarithm = new wxCheckBox (this, CHECK_STREAMFLOW_LOGARITHM, wxT("Use logarithm for streamflow in .RCH file"));
	stack_right->Add (check_streamflow_logarithm, 0, wxALL, 5);

	// Row 5 left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical precip. records per file:")), 0, wxEXPAND | wxALL, 5);
	small_row->AddStretchSpacer (1);
	edit_stations_per_file_1 = new integer_edit (this, EDIT_STATIONS_PER_FILE_1, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_stations_per_file_1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack_left->Add(small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 5 right
	check_print_pesticide_output = new wxCheckBox (this, CHECK_PRINT_PESTICIDE_OUTPUT, wxT("Print pesticide output"));
	stack_right->Add (check_print_pesticide_output, 0, wxALL, 5);

	// Row 6 left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical temp. records per file:")), 0, wxEXPAND | wxALL, 5);
	small_row->AddStretchSpacer ();
	edit_stations_per_file_2 = new integer_edit (this, EDIT_STATIONS_PER_FILE_2, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	small_row->Add (edit_stations_per_file_2, 0, wxEXPAND | wxALL, 5);
	stack_left->Add (small_row, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 6 right
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->AddStretchSpacer ();
	small_row->Add (new wxButton (this, BUTTON_BASIN, wxT("Basin Data")), 0, wxALIGN_BOTTOM);
	stack_right->Add (small_row, 0, wxEXPAND | wxALL, 5);

	// Row 7 left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Watershed area, km^2:")), 0, wxALL, 5);
	small_row->AddStretchSpacer ();
	static_area = new double_static (this, STATIC_AREA, wxT("0.0"));
	small_row->Add(static_area, 0, wxALL, 5);
	stack_left->Add(small_row, 0, wxALL, 5);

	// Row 7 right
	list_routes = new wxListBox (this, LIST_ROUTES, wxDefaultPosition, wxSize (200, 200));
	stack_right->Add (list_routes, 1, wxEXPAND | wxALL);

	// Row 8 left
	list_subbasins = new wxListBox (this, LIST_SUBBASINS, wxDefaultPosition, wxSize (200, 200));
	stack_left->Add (list_subbasins, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Row 8 right
	stack_right->Add (new wxButton (this, BUTTON_EDIT_ROUTE, wxT("Edit")), 0, wxEXPAND | wxALL, 5);

	// Row 9 left
	small_row = new wxBoxSizer (wxHORIZONTAL);
	small_row->AddStretchSpacer ();
	small_row->Add (new wxButton (this, BUTTON_EDIT, wxT("Edit")), 0, wxEXPAND | wxALL, 5);
	small_row->AddStretchSpacer ();
	small_row->Add (new wxButton (this, BUTTON_RESIZE_SUBBASINS, wxT("Resize")), 0, wxEXPAND | wxALL, 5);
	stack_left->Add (small_row);

	// Row 9 right
	stack_right->Add (new wxButton (this, BUTTON_RENUMBER_HRUS, wxT("Renumber HRUs")), 0, wxEXPAND | wxALL, 5);
	row->Add (stack_left);
	row->Add (stack_right);

	// stack->Add (row, 1, wxALL);
	stack->Add (row);
	SetSizerAndFit(stack);
}

BEGIN_EVENT_TABLE(page_control_record, wxPanel)
	EVT_BUTTON (BUTTON_LANDUSE_REPORT, page_control_record::OnLanduseReport)
	EVT_BUTTON (BUTTON_BASIN, page_control_record::OnBasin)
	EVT_BUTTON (BUTTON_EDIT, page_control_record::OnEdit)
	EVT_BUTTON (BUTTON_RESIZE_SUBBASINS, page_control_record::OnResizeSubbasins)
	EVT_BUTTON (BUTTON_RENUMBER_HRUS, page_control_record::OnRenumberHrus)
	EVT_BUTTON (BUTTON_EDIT_ROUTE, page_control_record::OnEditRoute)
	EVT_LISTBOX_DCLICK (LIST_ROUTES, page_control_record::OnDblClkRoute)
	EVT_LISTBOX_DCLICK (LIST_SUBBASINS, page_control_record::OnDblClickSubbasin)
END_EVENT_TABLE()


void page_control_record::OnDblClickSubbasin (wxCommandEvent& e)

{
    OnEdit (e);
}

bool page_control_record::TransferDataToWindow ()

{
	edit_description->SetValue (control->Description);
	static_id->Set (control->ID);
	edit_run_length->Set (control->RunLength);
	edit_first_year->Set (control->FirstYear);
	edit_first_julian_day->Set (control->FirstJulianDay);
	edit_last_julian_day->Set (control->LastJulianDay);
	edit_stations_per_file_1->Set (control->StationsPerFilePCP);
	edit_stations_per_file_2->Set (control->StationsPerFileTMP);
	combo_print_code->SetSelection (control->PrintCode);
	edit_skip_years->Set ((int) control->SkipYears);
	static_area->Set (control->total_area_km2 ());
	check_condense_std_output->SetValue (control->CondenseSTDFile == VARIANT_TRUE);
	check_streamflow_logarithm->SetValue (control->StreamflowLogarithm == VARIANT_TRUE);
	check_print_pesticide_output->SetValue (control->PrintPesticideOutput == VARIANT_TRUE);

	show_subbasins ();
	show_routes ();
	/*
    if (cfg->main_file_definition.path.get_length () > 0)
        edit_data_file->WriteText (cfg->main_file_definition.path.get_text_ascii ());
    else
        edit_data_file->Clear ();

    if (cfg->cfgpath.get_length () > 0)
        static_config_file->SetLabel (cfg->cfgpath.get_text_ascii ());
    else
        static_config_file->SetLabel ("");

    if (cfg->main_file_definition.is_delimited) {
        radio_fixed->SetValue (true);
        radio_variable->SetValue (false);
    }
    else {
        radio_fixed->SetValue (false);
        radio_variable->SetValue (true);
    }
	*/
	return true;
}

void page_control_record::OnLanduseReport
	(wxCommandEvent &)
{
	dynamic_string report;
	control_record_report reporter;

	reporter.create (control, controller->soils, report);

	report.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Control Record Report", L"", report);
	d.ShowWindowModal ();
}

void page_control_record::OnBasin
	(wxCommandEvent &)

{
	propsheet_basin pb (this, control, controller, help_list, db);
	if (pb.ShowModal () == wxID_OK) {

	}
}

void page_control_record::OnEdit
	(wxCommandEvent &)
{
	int index;
	SubBasin *sub;

	if ((index = list_subbasins->GetSelection ()) != -1) {

		sub = (SubBasin *) control->subbasins.find_by_index (index);

		propsheet_subbasin ns (this, sub, control, controller, help_list, db);
		if (ns.ShowModal () == wxID_OK)
			show_subbasins ();
	}
}

void page_control_record::OnResizeSubbasins
	(wxCommandEvent &)
{
}

void page_control_record::OnRenumberHrus
	(wxCommandEvent &)
{
}

bool page_control_record::TransferDataFromWindow ()

{
	strcpy_s (control->Description, 128, edit_description->GetValue ().ToAscii ());
	control->RunLength = edit_run_length->Get ();
	control->FirstYear = edit_first_year->Get ();
	control->FirstJulianDay = edit_first_julian_day->Get ();
	control->LastJulianDay = edit_last_julian_day->Get ();
	control->StationsPerFilePCP = edit_stations_per_file_1->Get ();
	control->StationsPerFileTMP = edit_stations_per_file_2->Get ();
	control->PrintCode = combo_print_code->GetSelection ();
	control->SkipYears = edit_skip_years->Get ();
	control->CondenseSTDFile = check_condense_std_output->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;
	control->StreamflowLogarithm = check_streamflow_logarithm->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;
	control->PrintPesticideOutput = check_print_pesticide_output->GetValue () ? VARIANT_TRUE : VARIANT_FALSE;

	return true;
}

void page_control_record::show_routes ()

{
	std::vector <dynamic_string> lines;
	std::vector <dynamic_string>::iterator line;
	std::vector <long>::const_iterator data;
	int nItem;

	list_routes->Clear ();
	if (control->routing.outlet)
		control->routing.outlet->write_list (&lines, &routing_data);

	for (line = lines.begin (), data = routing_data.begin ();
	line != lines.end () && data != routing_data.end ();
	++line, ++data) {
		nItem = list_routes->Append (line->get_text_wide ());
		list_routes->SetClientData (nItem, (void *) &*data);
	}
}

void page_control_record::show_subbasins ()

{
	dynamic_string data;
	std::vector <SubBasin>::const_iterator sub;

	list_subbasins->Clear ();
	for (sub = control->subbasins.basins.begin ();
	sub != control->subbasins.basins.end ();
	++sub) {
		data.format ("%d ", sub->Subbasin);
		data += sub->Description;
		list_subbasins->Append (data.get_text_wide ());
	}
}

void page_control_record::OnDblClkRoute
	(wxCommandEvent &e)
{
	OnEditRoute (e);
}

void page_control_record::OnEditRoute
	(wxCommandEvent &)
{
	int index;
	SetCursor (*wxHOURGLASS_CURSOR);
	if ((index = list_routes->GetSelection ()) != -1) {
		long *hsl;

		hsl = (long *) list_routes->GetClientData (index);
		swat_routing_node *node;
		if ((node = control->routing.match_hsl (*hsl)) != NULL) {
			SetCursor (*wxSTANDARD_CURSOR);
			propsheet_routing d (this, node, control, controller);
			if (d.ShowModal () == wxID_OK)
				show_routes ();
		}
	}
	else
		SetCursor (*wxSTANDARD_CURSOR);
}

