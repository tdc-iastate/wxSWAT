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
#include "page_subbasin_climate.h"

page_subbasin_climate::page_subbasin_climate
	(wxWindow *parent,
	const int id,
	SubBasin *pcontrol)
	: wxPanel (parent, id)

{
	wxBoxSizer *columns [4], *row;

	basin = pcontrol;

	row = new wxBoxSizer( wxHORIZONTAL );

	columns[0] = new wxBoxSizer(wxVERTICAL);
	columns[1] = new wxBoxSizer(wxVERTICAL);
	columns[2] = new wxBoxSizer(wxVERTICAL);
	columns[3] = new wxBoxSizer(wxVERTICAL);
	columns [0]->Add(new wxStaticText(this, wxID_ANY, "Rainfall\nAdjustment", wxDefaultPosition, wxDefaultSize));
	columns [1]->Add(new wxStaticText(this, wxID_ANY, "Temperature\nAdjustment", wxDefaultPosition, wxDefaultSize));
	columns [2]->Add(new wxStaticText(this, wxID_ANY, "Radiation\nAdjustment", wxDefaultPosition, wxDefaultSize));
	columns [3]->Add(new wxStaticText(this, wxID_ANY, "Humidity\nAdjustment", wxDefaultPosition, wxDefaultSize));
	for (int month_index = 0; month_index < 12; ++month_index) {
		edit_rainfalladjustment[month_index] = new float_edit(this, EDIT_RAINFALLADJUSTMENT1 + month_index * 4, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		columns [0]->Add(edit_rainfalladjustment[month_index]);
		edit_temperatureadjustment[month_index] = new float_edit(this, EDIT_TEMPERATUREADJUSTMENT1 + month_index * 4, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		columns[1]->Add(edit_temperatureadjustment[month_index]);
		edit_radiationadjustment[month_index] = new float_edit(this, EDIT_RADIATIONADJUSTMENT1 + month_index * 4, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		columns [2]->Add(edit_radiationadjustment[month_index]);
		edit_humidityadjustment[month_index] = new float_edit(this, EDIT_HUMIDITYADJUSTMENT1 + month_index * 4, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		columns [3]->Add(edit_humidityadjustment[month_index]);
	}
	row->Add(columns[0]);
	row->AddSpacer (10);
	row->Add(columns[1]);
	row->AddSpacer(10);
	row->Add(columns[2]);
	row->AddSpacer(10);
	row->Add(columns[3]);

	SetSizerAndFit(row);

	set_controls ();

	// edit_help->SetValue (basin->Help.get_text ());

}

BEGIN_EVENT_TABLE(page_subbasin_climate, wxPanel)
END_EVENT_TABLE()

void page_subbasin_climate::set_controls ()

{
	for (int month_index = 0; month_index < 12; ++month_index) {
		edit_rainfalladjustment [month_index]->Set (basin->RainfallAdjustment [month_index]);
		edit_temperatureadjustment [month_index]->Set (basin->TemperatureAdjustment [month_index]);
		edit_radiationadjustment [month_index]->Set (basin->RadiationAdjustment [month_index]);
		edit_humidityadjustment [month_index]->Set (basin->HumidityAdjustment [month_index]);
	}
}

bool page_subbasin_climate::read_controls ()

{
	for (int month_index = 0; month_index < 12; ++month_index) {
		basin->RainfallAdjustment [month_index] = edit_rainfalladjustment [month_index]->Get ();
		basin->TemperatureAdjustment [month_index] = edit_temperatureadjustment [month_index]->Get ();
		basin->RadiationAdjustment [month_index] = edit_radiationadjustment [month_index]->Get ();
		basin->HumidityAdjustment [month_index] = edit_humidityadjustment [month_index]->Get ();
	}
	return true;
}

