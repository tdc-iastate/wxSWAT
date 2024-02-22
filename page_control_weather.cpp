#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/Timestamp.h"
#include "../util/custom.h"
#include "../util/device_coordinate.h"
// #include "bitmaps/wizard_control_record.xpm"
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
#include "page_control_weather.h"
#include "gridwidget.h"

BEGIN_EVENT_TABLE(page_control_weather, wxPanel)
	// EVT_NOTEBOOK_PAGE_CHANGING (wxID_ANY, page_control_weather::OnSelChange) 
	// EVT_NOTEBOOK_PAGE_CHANGED (wxID_ANY, page_control_weather::OnPageChanged) 
END_EVENT_TABLE()

page_control_weather::page_control_weather
	(wxWindow *parent,
	class SWATControlRecord *p_control)
	: wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)

{
	control = p_control;

	wxString items_input_code [2];
	items_input_code [0] = "1 Measured data read for each subbasin";
	items_input_code [1] = "2 Data generated for each subbasin";

	wxBoxSizer *stack, *row;
	stack = new wxBoxSizer( wxVERTICAL );

	stack->AddSpacer (10);

	// Row 1
	row = new wxBoxSizer( wxHORIZONTAL );
	row->AddSpacer (25);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Random generator seed:")), 0, wxEXPAND | wxALL, 5);
	row->AddStretchSpacer ();
	edit_random_generator_seed = new integer_edit (this, EDIT_RANDOM_GENERATOR_SEED, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_random_generator_seed, 0, wxEXPAND | wxALL, 5);
	row->AddStretchSpacer ();
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 2
	{
		row = new wxBoxSizer( wxHORIZONTAL );
		row->AddSpacer (25);
		row->Add (new wxStaticText (this, wxID_ANY, wxT("Sub-daily measurement timestep, minutes:")));
		wxString items [12];
		items [0] = "0 None";
		items [1] = "1 Minute";
		items [2] = "2 Minutes";
		items [3] = "3 Minutes";
		items [4] = "4 Minutes";
		items [5] = "5 Minutes";
		items [6] = "6 Minutes";
		items [7] = "10 Minutes";
		items [8] = "12 Minutes";
		items [9] = "15 Minutes";
		items [10] = "20 Minutes";
		items [11] = "30 Minutes";

		row->AddStretchSpacer (1);
		combo_subdaily_input_timestep = new wxComboBox (this, COMBO_SUBDAILY_INPUT_TIMESTEP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 12, items, wxCB_DROPDOWN);
		row->Add (combo_subdaily_input_timestep);
		row->AddSpacer (25);
		stack->Add (row, 1, wxEXPAND | wxALL);
	}

	// Row 3
	row = new wxBoxSizer( wxHORIZONTAL );
	row->AddSpacer (25);
	check_mixed_exponential_distribution = new wxCheckBox (this, CHECK_MIXED_EXPONENTIAL_DISTRIBUTION, wxT("Mixed exponential distribution"));
	row->Add (check_mixed_exponential_distribution);
	stack->Add (row, 0);

	// Row 4
	row = new wxBoxSizer( wxHORIZONTAL );
	row->AddSpacer (25);
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Mixed exponential exponent:")));
	edit_mixed_exponential_exponent = new float_edit (this, EDIT_MIXED_EXPONENTIAL_EXPONENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->AddStretchSpacer (1);
	row->Add (edit_mixed_exponential_exponent);
	row->AddSpacer (25);
	stack->Add (row, 1, wxEXPAND | wxALL);

	stack->AddSpacer (10);

	{
		// row 5
		row = new wxBoxSizer( wxHORIZONTAL );
		row->Add (new wxStaticText (this, wxID_ANY, wxT("Rainfall input code:")));
		row->AddStretchSpacer (1);
		combo_rainfall_input_code = new wxComboBox (this, COMBO_RAINFALL_INPUT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, items_input_code, wxCB_DROPDOWN);
		row->Add (combo_rainfall_input_code);
		stack->Add (row, 1, wxEXPAND | wxALL);

		// Row 6
		row = new wxBoxSizer( wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, wxT("Temperature input code:")));
		row->AddStretchSpacer (1);
		combo_temperature_input_code = new wxComboBox (this, COMBO_TEMPERATURE_INPUT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, items_input_code, wxCB_DROPDOWN);
		row->Add (combo_temperature_input_code);
		stack->Add (row, 1, wxEXPAND | wxALL);
	
		// Row 7
		row = new wxBoxSizer( wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, wxT("Solar radiation input code:")));
		row->AddStretchSpacer (1);
		combo_solar_radiation_input_code = new wxComboBox (this, COMBO_SOLAR_RADIATION_INPUT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, items_input_code, wxCB_DROPDOWN);
		row->Add (combo_solar_radiation_input_code);
		stack->Add (row, 1, wxEXPAND | wxALL);

		// Row 8
		row = new wxBoxSizer( wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, wxT("Relative humidity input code:")));
		row->AddStretchSpacer (1);
		combo_relative_humidity_input_code = new wxComboBox (this, COMBO_RELATIVE_HUMIDITY_INPUT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, items_input_code, wxCB_DROPDOWN);
		row->Add (combo_relative_humidity_input_code);
		stack->Add (row, 1, wxEXPAND | wxALL);

		// Row 9
		row = new wxBoxSizer( wxHORIZONTAL);
		row->Add (new wxStaticText (this, wxID_ANY, wxT("Wind speed input code:")));
		row->AddStretchSpacer (1);
		combo_wind_speed_input_code = new wxComboBox (this, COMBO_WIND_SPEED_INPUT_CODE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, items_input_code, wxCB_DROPDOWN);
		row->Add (combo_wind_speed_input_code);
		stack->Add (row, 1, wxEXPAND | wxALL);
	}

	edit_help = new wxTextCtrl (this, EDIT_HELP_WEATHER, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help, 1, wxEXPAND | wxALL);

	SetSizerAndFit(stack);

	edit_random_generator_seed->Set (control->RandomGeneratorSeed);
	combo_rainfall_input_code->SetSelection (control->RainfallInputCode - 1);

	switch (control->SubDailyRainfallTimestep) {
		case 10:
			combo_subdaily_input_timestep->SetSelection (6);
			break;
		case 12:
			combo_subdaily_input_timestep->SetSelection (7);
			break;
		case 15:
			combo_subdaily_input_timestep->SetSelection (8);
			break;
		case 20:
			combo_subdaily_input_timestep->SetSelection (9);
			break;
		case 30:
			combo_subdaily_input_timestep->SetSelection (10);
			break;
		default: // 0..6 minutes
			combo_subdaily_input_timestep->SetSelection (control->SubDailyRainfallTimestep);
	}
	check_mixed_exponential_distribution->SetValue (control->MixedExponentialDistribution == VARIANT_TRUE);
	edit_mixed_exponential_exponent->Set (control->MixedExponentialExponent);
	combo_temperature_input_code->SetSelection (control->TemperatureInputCode - 1);
	combo_solar_radiation_input_code->SetSelection (control->SolarRadiationInputCode - 1);
	combo_relative_humidity_input_code->SetSelection (control->RelativeHumidityInputCode - 1);
	combo_wind_speed_input_code->SetSelection (control->WindSpeedInputCode - 1);
	// edit_help->SetValue (CLASS->Help.get_text ());

    // Connect(wxEVT_NOTEBOOK_PAGE_CHANGING, wxCommandEventHandler (page_control_weather::OnSelChange));

}

void page_control_weather::show_input ()

{
    SetSizerAndFit(GetSizer ());
}

void page_control_weather::fill_output_row
	(const int)

// first call must use row_index=0

{
}

bool page_control_weather::read_controls ()

{

	control->RandomGeneratorSeed = edit_random_generator_seed->Get ();
	control->RainfallInputCode = combo_rainfall_input_code->GetSelection () + 1;

	switch (combo_subdaily_input_timestep->GetSelection ()) {
		case 6:
			control->SubDailyRainfallTimestep = 10;
			break;
		case 7:
			control->SubDailyRainfallTimestep = 12;
			break;
		case 8:
			control->SubDailyRainfallTimestep = 15;
			break;
		case 9:
			control->SubDailyRainfallTimestep = 20;
			break;
		case 10:
			control->SubDailyRainfallTimestep = 30;
			break;
		default: // 0..6 minutes
			control->SubDailyRainfallTimestep = combo_subdaily_input_timestep->GetSelection ();
	}

	control->MixedExponentialDistribution = check_mixed_exponential_distribution->GetValue ();
	control->MixedExponentialExponent = edit_mixed_exponential_exponent->Get ();
	control->TemperatureInputCode = combo_temperature_input_code->GetSelection () + 1;
	control->SolarRadiationInputCode = combo_solar_radiation_input_code->GetSelection () + 1;
	control->RelativeHumidityInputCode = combo_relative_humidity_input_code->GetSelection () + 1;
	control->WindSpeedInputCode = combo_wind_speed_input_code->GetSelection () + 1;

	return true;
}


