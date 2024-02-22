#include "../util/utility_afx.h"
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <mutex>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/propdlg.h>
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
#include "../util/custom.h"
#include "../util/dialog_error_list.h"
#include "../util/interface_window.h"
#include "../util/interface_window_wx.h"
#include "../util/list_field_help.h"
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
#include "../i_swat_database/swat_controller.h"
#include "../i_swat_database/SWATRetentionIrrigationBasin.h"
#include "../i_swat_database/SWATSedimentationFiltrationBasin.h"
#include "../i_swat_database/SWATStreamWaterQuality.h"
#include "../i_swat_database/SWATPond.h"
#include "../i_swat_database/SWATWaterUse.h"
#include "page_basin_1.h"
#include "../weather/averager_weighted.h"
#include "propsheet_hru.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(page_basin_1, wxPanel)
END_EVENT_TABLE()

page_basin_1::page_basin_1
	(wxWindow *parent,
	const int id,
	SWATControlRecord *pcontrol,
	swat_controller *pcontroller,
	list_field_help_swat *p_help,
	odbc_database *p_db)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row;

	control = pcontrol;
	controller = pcontroller;
	help = p_help;
	db = p_db;

	stack = new wxBoxSizer( wxVERTICAL );

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Snowfall temperature, deg. C:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowfalltemperature = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowfalltemperature, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Snow melt temperature, deg. C:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowmelttemperature = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowmelttemperature, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Snow melt rate, June 21:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowmeltratejune21 = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowmeltratejune21, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	row->Add (new wxStaticText (this, wxID_ANY, wxT("December 21:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowmeltratedecember21 = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowmeltratedecember21, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Snow pack temperature lag:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowpacktemperaturelag = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowpacktemperaturelag, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Snow cover minimum content, mm H2O:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowcoverminimumcontent = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowcoverminimumcontent, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Snow volume fraction for 50% cover:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_snowcovervolume50percent = new float_edit (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_snowcovervolume50percent, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	edit_help = new wxTextCtrl (this, EDIT_HELP, wxEmptyString, wxDefaultPosition, wxSize (400, 200));
	stack->Add (edit_help, 0, wxALL, 5);

	SetSizerAndFit(stack);
}


bool page_basin_1::TransferDataToWindow ()

{
	edit_snowfalltemperature->Set (control->SnowfallTemperature);
	edit_snowmelttemperature->Set (control->SnowMeltTemperature);
	edit_snowmeltratejune21->Set (control->SnowMeltRateJune21);
	edit_snowmeltratedecember21->Set (control->SnowMeltRateDecember21);
	edit_snowpacktemperaturelag->Set (control->SnowPackTemperatureLag);
	edit_snowcoverminimumcontent->Set (control->SnowCoverMinimumContent);
	edit_snowcovervolume50percent->Set (control->SnowCoverVolume50Percent);
	return true;
}

bool page_basin_1::TransferDataFromWindow ()

{
	control->SnowfallTemperature = edit_snowfalltemperature->Get ();
	control->SnowMeltTemperature = edit_snowmelttemperature->Get ();
	control->SnowMeltRateJune21 = edit_snowmeltratejune21->Get ();
	control->SnowMeltRateDecember21 = edit_snowmeltratedecember21->Get ();
	control->SnowPackTemperatureLag = edit_snowpacktemperaturelag->Get ();
	control->SnowCoverMinimumContent = edit_snowcoverminimumcontent->Get ();
	control->SnowCoverVolume50Percent = edit_snowcovervolume50percent->Get ();
	return true;
}

