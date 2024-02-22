#include "../util/utility_afx.h"
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <wx/frame.h>
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
#include "../util/device_coordinate.h"

#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"

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
#include "page_subbasin_tributary.h"

page_subbasin_tributary::page_subbasin_tributary
	(wxWindow *parent,
	const int id,
	SubBasin *pcontrol)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row;

	basin = pcontrol;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Initial snow water content, mm:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_initial_snow_water_content = new float_edit (this, EDIT_INITIAL_SNOW_WATER_CONTENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_initial_snow_water_content, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Tributary channel longest length, km:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_tributary_channel_longest_length = new float_edit (this, EDIT_TRIBUTARY_CHANNEL_LONGEST_LENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tributary_channel_longest_length, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Tributary channel average slope, m/m:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_tributary_channel_average_slope = new float_edit (this, EDIT_TRIBUTARY_CHANNEL_AVERAGE_SLOPE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tributary_channel_average_slope, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Tributary channel average width, m:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_tributary_channel_average_width = new float_edit (this, EDIT_TRIBUTARY_CHANNEL_AVERAGE_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tributary_channel_average_width, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Tributary channel hydraulic conductivity, mm/hr:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_tributary_channel_hydraulic_conductivity = new float_edit (this, EDIT_TRIBUTARY_CHANNEL_HYDRAULIC_CONDUCTIVITY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tributary_channel_hydraulic_conductivity, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Tributary channel Mannings n coefficient:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_tributary_channel_mannings_n_coefficient = new float_edit (this, EDIT_TRIBUTARY_CHANNEL_MANNINGS_N_COEFFICIENT, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_tributary_channel_mannings_n_coefficient, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Carbon dioxide concentration, ppmv:")), 0, wxALIGN_BOTTOM);
	row->AddStretchSpacer (1);
	edit_carbon_dioxide_concentration = new float_edit (this, EDIT_CARBON_DIOXIDE_CONCENTRATION, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_carbon_dioxide_concentration, 0, wxALIGN_BOTTOM);
	stack->Add (row, 0, wxALL, 5);

	edit_help = new wxTextCtrl (this, EDIT_HELP_TRIBUTARY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	stack->Add (edit_help, 0, wxALL, 5);

	SetSizerAndFit(stack);

	set_controls ();

	// edit_help->SetValue (basin->Help.get_text ());


	SetSizerAndFit(stack);

}

BEGIN_EVENT_TABLE(page_subbasin_tributary, wxPanel)
END_EVENT_TABLE()

void page_subbasin_tributary::set_controls ()

{
	edit_initial_snow_water_content->Set (basin->InitialSnowWaterContent);
	edit_tributary_channel_longest_length->Set (basin->TributaryLongestLength);
	edit_tributary_channel_average_slope->Set (basin->TributaryAverageSlope);
	edit_tributary_channel_average_width->Set (basin->TributaryAverageWidth);
	edit_tributary_channel_hydraulic_conductivity->Set (basin->TributaryAlluviumK);
	edit_tributary_channel_mannings_n_coefficient->Set (basin->TributaryManningsN);
	edit_carbon_dioxide_concentration->Set (basin->CarbonDioxideConcentration);
}

bool page_subbasin_tributary::read_controls ()

{
	basin->InitialSnowWaterContent = edit_initial_snow_water_content->Get ();
	basin->TributaryLongestLength = edit_tributary_channel_longest_length->Get ();
	basin->TributaryAverageSlope = edit_tributary_channel_average_slope->Get ();
	basin->TributaryAverageWidth = edit_tributary_channel_average_width->Get ();
	basin->TributaryAlluviumK = edit_tributary_channel_hydraulic_conductivity->Get ();
	basin->TributaryManningsN = edit_tributary_channel_mannings_n_coefficient->Get ();
	basin->CarbonDioxideConcentration = edit_carbon_dioxide_concentration->Get ();

	return true;
}

