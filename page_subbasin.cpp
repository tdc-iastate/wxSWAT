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
#include "page_subbasin.h"
#include "../weather/averager_weighted.h"
#include "propsheet_hru.h"
#include "list_field_help_swat.h"

BEGIN_EVENT_TABLE(page_subbasin, wxPanel)
	EVT_CHECKBOX (CHECK_POND, page_subbasin::OnCheckPond)
	EVT_CHECKBOX (CHECK_SWQ, page_subbasin::OnCheckSWQ)
	EVT_CHECKBOX (CHECK_SWQ, page_subbasin::OnCheckWaterUse)
	EVT_BUTTON (BUTTON_POND, page_subbasin::OnPond)
	EVT_BUTTON (BUTTON_SWQ, page_subbasin::OnSwq)
	EVT_BUTTON (BUTTON_WATER_USE, page_subbasin::OnWaterUse)
	EVT_BUTTON (BUTTON_DELETE, page_subbasin::OnDelete)
	EVT_BUTTON (BUTTON_EDIT, page_subbasin::OnEdit)
	EVT_BUTTON (BUTTON_MOVE, page_subbasin::OnMove)
	EVT_BUTTON (BUTTON_WEATHER_SUMMARY, page_subbasin::OnWeatherSummary)
	EVT_LISTBOX_DCLICK (LIST_HRU, page_subbasin::OnDblClickHRU)
END_EVENT_TABLE()

page_subbasin::page_subbasin
	(wxWindow *parent,
	const int id,
	SubBasin *pbasin,
	SWATControlRecord *pcontrol,
	swat_controller *pcontroller,
	list_field_help_swat *p_help,
	odbc_database *p_db)
	: wxPanel (parent, id)

{
	wxBoxSizer *stack, *row, *small_row, *column;

	basin = pbasin;
	control = pcontrol;
	controller = pcontroller;
	help = p_help;
	db = p_db;

	stack = new wxBoxSizer( wxVERTICAL );

	edit_description = new wxTextCtrl (this, EDIT_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize (300, 24));
	stack->Add (edit_description, 0, wxALL, 5);

	// Row 2
	row = new wxBoxSizer( wxHORIZONTAL );
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Latitude:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_latitude = new float_edit (this, EDIT_LATITUDE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_latitude, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	row->Add (0, 0, wxSizerFlags ().Expand ());
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Longitude:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_longitude = new float_edit (this, EDIT_LONGITUDE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_longitude, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	row->Add (0, 0, wxSizerFlags ().Expand ());
	row->Add (new wxStaticText (this, wxID_ANY, wxT("Elevation:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	row->AddStretchSpacer ();
	edit_elevation = new float_edit (this, EDIT_ELEVATION, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	row->Add (edit_elevation, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	stack->Add (row, 0, wxEXPAND | wxALL, 5);

	// Row 3

	{
		row = new wxBoxSizer( wxHORIZONTAL );
		column = new wxBoxSizer( wxVERTICAL );

		small_row = new wxBoxSizer( wxHORIZONTAL );
		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Precipitation lapse rate, mm H20/km:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddStretchSpacer (1);
		edit_precipitation_lapse_rate = new float_edit (this, EDIT_PRECIPITATION_LAPSE_RATE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		small_row->Add (edit_precipitation_lapse_rate, 0, wxALIGN_BOTTOM);
		column->Add (small_row, 0, wxALL, 5);

		small_row = new wxBoxSizer( wxHORIZONTAL );
		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Temperature lapse rate, deg C/km:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddStretchSpacer (1);
		edit_temperature_lapse_rate = new float_edit (this, EDIT_TEMPERATURE_LAPSE_RATE, wxEmptyString, wxDefaultPosition, wxDefaultSize);
		small_row->Add (edit_temperature_lapse_rate, 0, wxALIGN_BOTTOM);
		column->Add (small_row, 0, wxALL, 5);

		row->Add (column);

		column = new wxBoxSizer( wxVERTICAL );
		small_row = new wxBoxSizer( wxHORIZONTAL );
		static_area_acres = new wxStaticText (this, STATIC_AREA_ACRES, wxT(""));
		small_row->Add (static_area_acres, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddStretchSpacer (1);
		static_area_km2 = new wxStaticText (this, STATIC_AREA_KM2, wxT(""));
		small_row->Add (static_area_km2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		// static_area_fraction = new wxStaticText (this, STATIC_AREA_FRACTION, wxT(""));
		// stack->Add (static_area_fraction, 0, wxALIGN_BOTTOM);
		column->Add (small_row, 0, wxALL, 5);

		row->Add (column);
		stack->Add (row);
	}

	{
		// Row 4
		row = new wxBoxSizer( wxHORIZONTAL );
		wxStaticBoxSizer *outline_box = new wxStaticBoxSizer (wxVERTICAL, this, wxT("Weather:"));

		small_row = new wxBoxSizer( wxHORIZONTAL );
		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Generated:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		static_weather_station = new wxStaticText (this, STATIC_WEATHER_STATION, wxT(""), wxDefaultPosition, wxSize (200, 24));
		small_row->Add (static_weather_station, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		small_row->Add(new wxButton(this, BUTTON_WEATHER_SUMMARY, wxT("Summary")), 0, wxALL, 5);
		outline_box->Add(small_row);

		small_row = new wxBoxSizer( wxHORIZONTAL );
		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical precipitation:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		static_weather_precipitation = new wxStaticText (this, STATIC_RECORDED_WEATHER, wxT(""));
		small_row->Add (static_weather_precipitation, 0, wxALIGN_BOTTOM | wxALL, 5);
		small_row->AddSpacer(15);

		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical temperature:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		static_weather_temperature = new wxStaticText (this, STATIC_WEATHER_TEMPERATURE, wxT(""));
		small_row->Add (static_weather_temperature, 0, wxALIGN_BOTTOM | wxALL, 5);
		outline_box->Add(small_row);

		small_row = new wxBoxSizer( wxHORIZONTAL );
		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical solar radiation:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		static_weather_solar_radiation = new wxStaticText (this, STATIC_WEATHER_SOLAR_RADIATION, wxT(""));
		small_row->Add (static_weather_solar_radiation, 0, wxALIGN_BOTTOM | wxALL, 5);
		small_row->AddSpacer(15);

		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical rel. humidity:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		static_weather_relative_humidity = new wxStaticText (this, STATIC_WEATHER_RELATIVE_HUMIDITY, wxT(""));
		small_row->Add (static_weather_relative_humidity, 0, wxALIGN_BOTTOM | wxALL, 5);
		outline_box->Add(small_row);

		small_row = new wxBoxSizer( wxHORIZONTAL );
		small_row->Add (new wxStaticText (this, wxID_ANY, wxT("Historical wind speed:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		small_row->AddSpacer (15);
		static_weather_wind_speed = new wxStaticText (this, STATIC_WEATHER_WIND_SPEED, wxT(""));
		small_row->Add (static_weather_wind_speed, 0, wxALIGN_BOTTOM | wxALL, 5);
		outline_box->Add(small_row);

		row->Add (outline_box, 1, wxEXPAND);

		stack->Add (row);
	}

	// Row 5
	row = new wxBoxSizer( wxHORIZONTAL );
	check_pond = new wxCheckBox (this, CHECK_POND, wxT("Has pond information:"));
	row->Add (check_pond);
	row->AddStretchSpacer (1);
	button_pond = new wxButton (this, BUTTON_POND, wxT("Pond"));
	row->Add (button_pond);
	row->AddSpacer (50);
	stack->Add (row, 0, wxALL, 5);

	// Row 6
	row = new wxBoxSizer( wxHORIZONTAL );
	check_swq = new wxCheckBox (this, CHECK_SWQ, wxT("Has stream quality information"));
	row->Add (check_swq);
	row->AddSpacer (10);
	button_swq = new wxButton (this, BUTTON_SWQ, wxT("Stream Quality"));
	row->Add (button_swq);
	stack->Add(row, 0, wxALL, 5);

	// Row 7
	row = new wxBoxSizer( wxHORIZONTAL );
	check_water_use = new wxCheckBox (this, CHECK_WATER_USE, wxT("Has water use information"));
	row->Add (check_water_use);
	row->AddSpacer(10);
	button_water_use = new wxButton (this, BUTTON_WATER_USE, wxT("Water Use"));
	row->Add (button_water_use);
	stack->Add (row, 0, wxALL, 5);

	// Row 8
	wxStaticBoxSizer *outline_box = new wxStaticBoxSizer (wxHORIZONTAL, this, wxT("HRUs"));
	row = new wxBoxSizer( wxHORIZONTAL );
	list_hru = new wxListBox (this, LIST_HRU, wxDefaultPosition, wxSize (400, 200));
	row->Add (list_hru, 9, wxEXPAND);
	// column to right of list_hru
	column = new wxBoxSizer( wxVERTICAL );
	column->Add (new wxButton (this, BUTTON_DELETE, wxT("Delete")), 0, wxALL, 5);
	column->Add (new wxButton (this, BUTTON_EDIT, wxT("Edit")), 0, wxALL, 5);
	column->Add (new wxButton (this, BUTTON_MOVE, wxT("Move")), 0, wxALL, 5);
	row->Add (column, 2, wxEXPAND);
	outline_box->Add (row, 0, wxALL, 5);
	stack->Add (outline_box, 0, wxGROW | wxALL, 5);

	SetSizerAndFit(stack);

	edit_description->SetValue (basin->Description);
	edit_latitude->Set (basin->Latitude);
	edit_longitude->Set (basin->Longitude);
	edit_elevation->Set (basin->Elevation);
	edit_precipitation_lapse_rate->Set (basin->PrecipitationLapseRate);
	edit_temperature_lapse_rate->Set (basin->TemperatureLapseRate);
	// static_area_fraction->SetValue (basin->AreaFraction.get_text ());

	check_pond->SetValue (basin->pond != NULL);
	check_swq->SetValue (basin->stream_water_quality != NULL);
	check_water_use->SetValue (basin->water_use != NULL);

	// button_water_use.EnableWindow (basin->water_use != NULL);

	SWATWeatherStations *weather;
	SWAT_Recorded_Weather *recorded_weather;
	dynamic_string data;

	if ((weather = controller->swat_weather->match (basin->WeatherStationID)) != NULL) {
		data.format ("%ld ", weather->WeatherStationID);
		data += weather->Name;
	}
	else
		data.format ("%ld", basin->WeatherStationID);
	static_weather_station->SetLabel (data.get_text_wide ());

	// Precip
	if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDPrecipitation)) != NULL) {
		data.format ("%ld ", recorded_weather->ID);
		data += recorded_weather->Name;
	}
	else
		data.format ("%ld", basin->WeatherIDPrecipitation);
	static_weather_precipitation->SetLabel (data.get_text_wide ());

	// Temperature
	if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDTemperature)) != NULL) {
		data.format ("%ld ", recorded_weather->ID);
		data += recorded_weather->Name;
	}
	else
		data.format ("%ld", basin->WeatherIDTemperature);
	static_weather_temperature->SetLabel (data.get_text_wide ());

	// Solar Radiation
	if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDSolarRadiation)) != NULL) {
		data.format ("%ld ", recorded_weather->ID);
		data += recorded_weather->Name;
	}
	else
		data.format ("%ld", basin->WeatherIDSolarRadiation);
	static_weather_solar_radiation->SetLabel (data.get_text_wide ());

	// Relative Humidity
	if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDRelativeHumidity)) != NULL) {
		data.format ("%ld ", recorded_weather->ID);
		data += recorded_weather->Name;
	}
	else
		data.format ("%ld", basin->WeatherIDRelativeHumidity);
	static_weather_relative_humidity->SetLabel (data.get_text_wide ());

	// Wind Speed
	if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDWindSpeed)) != NULL) {
		data.format ("%ld ", recorded_weather->ID);
		data += recorded_weather->Name;
	}
	else
		data.format ("%ld", basin->WeatherIDWindSpeed);
	static_weather_wind_speed->SetLabel (data.get_text_wide ());

	show_area_fraction ();
	show_hru_list ();

	wxCommandEvent empty_event;
	OnCheckPond (empty_event);
	// OnPond ();
	// OnSWQ ();
}

void page_subbasin::show_area_fraction ()

// Display the absolue area (acres, km^2) and fraction of subbasin
// represented by all HRUs' FractionofTotalWatershedArea

{
	double subbasin_total;
	dynamic_string description;

	subbasin_total = basin->total_area_km2 ();

	// convert km^2 to acres
	description.format ("%.2lf acres", subbasin_total * 247.105383);
	static_area_acres->SetLabelText (description.get_text_wide ());

	description.format ("%.2lf km^2", subbasin_total);
	static_area_km2->SetLabelText (description.get_text_wide ());
}

void page_subbasin::OnDblClickHRU (wxCommandEvent& e)

{
    OnEdit (e);
}

void page_subbasin::set_controls ()

{
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

}

/*
void page_subbasin::EndModal
	(int retCode)

{
	if (retCode == wxID_OK) {
		CLASS->Description = edit_description->GetValue ().ToAscii ();
		CLASS->Latitude = edit_latitude->GetValue ().ToAscii ();
		CLASS->Longitude = edit_longitude->GetValue ().ToAscii ();
		CLASS->Elevation = edit_elevation->GetValue ().ToAscii ();
		CLASS->PrecipitationLapseRate = edit_precipitation_lapse_rate->GetValue ().ToAscii ();
		CLASS->TemperatureLapseRate = edit_temperature_lapse_rate->GetValue ().ToAscii ();
		CLASS->Pond = check_pond->GetValue ();
		CLASS->Swq = check_swq->GetValue ();
		CLASS->WaterUse = check_water_use->GetValue ();
	}
	wxDialog::EndModal (retCode);
}
*/

void page_subbasin::OnCheckPond
	(wxCommandEvent &)
{
	if (check_pond->GetValue () == 1) {
		if (basin->pond == NULL)
			basin->pond = new SWATPond;
		button_pond->Enable ();
	}
	else {
		if (basin->pond != NULL) {
			delete basin->pond;
			basin->pond = NULL;
		}
		button_pond->Disable ();
	}
}

void page_subbasin::OnPond
	(wxCommandEvent &)

{
}

void page_subbasin::OnSwq
	(wxCommandEvent &)

{
}

void page_subbasin::OnCheckSWQ
	(wxCommandEvent &)
{
	if (check_swq->GetValue () == 1) {
		if (basin->stream_water_quality == NULL)
			basin->stream_water_quality = new SWATStreamWaterQuality;
		button_swq->Enable ();
	}
	else {
		if (basin->stream_water_quality != NULL) {
			delete basin->stream_water_quality;
			basin->stream_water_quality = NULL;
		}
		button_swq->Disable ();
	}
}

void page_subbasin::OnCheckWaterUse
	(wxCommandEvent &)
{
	if (check_water_use->GetValue () == 1) {
		if (basin->water_use == NULL)
			basin->water_use = new list_swat_water_use;
		button_water_use->Enable ();
	}
	else {
		if (basin->stream_water_quality != NULL) {
			delete basin->water_use;
			basin->water_use = NULL;
		}
		button_water_use->Disable ();
	}
}

void page_subbasin::OnWaterUse
	(wxCommandEvent &)
{
}

void page_subbasin::OnDelete
	(wxCommandEvent &)
{
}

void page_subbasin::OnEdit
	(wxCommandEvent &)
{
	int index;
	HRU_record *hru;

	SetCursor (*wxHOURGLASS_CURSOR);
	if ((index = list_hru->GetSelection ()) != -1) {
		hru = &basin->hru_list.hrus [index];

		propsheet_hru ph (this, hru, basin, control, controller, help, db); // doc->help_list);

		if (ph.ShowModal () == wxID_OK) {
		}
	}
}

void page_subbasin::OnMove
	(wxCommandEvent &)
{
}

void page_subbasin::OnWeatherSummary
	(wxCommandEvent &)
{
	SWATWeatherStations *weather;
	SWAT_Recorded_Weather *recorded_weather;
	averager_weighted report;
	dynamic_string log;

	interface_window_wx update_display;
	update_display.setup_wx (NULL);

	if ((weather = controller->swat_weather->match (basin->WeatherStationID)) != NULL) {
	}
	else
		log.add_formatted ("ERROR, generated weather station %ld not found.\n", basin->WeatherStationID);

	if (basin->WeatherIDPrecipitation != basin->WeatherIDTemperature)
		log += "Precipitation\n";
	// Precip
	if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDPrecipitation)) != NULL) {
		report.run (recorded_weather, basin->Subbasin, basin->total_area_km2 (), &update_display, log);
		report.report (log);
	}
	else
		log.add_formatted ("ERROR, historical precipitation ID %ld not found.\n", basin->WeatherIDPrecipitation);

	if (basin->WeatherIDPrecipitation != basin->WeatherIDTemperature) {
		log += "\n";
		log += "Temperature\n";

		// Temperature
		if ((recorded_weather = controller->swat_weather->match_historical (basin->WeatherIDTemperature)) != NULL) {
			report.run (recorded_weather, basin->Subbasin, basin->total_area_km2 (), &update_display, log);
			report.report (log);
		}
		else
			log.add_formatted ("ERROR, historical temperature ID %ld not found.\n", basin->WeatherIDTemperature);
	}

	log.convert_linefeeds_for_CEdit ();
	dialog_error_list d (this, L"Weather Summary", L"", log);
	d.ShowWindowModal ();
}

void page_subbasin::show_hru_list ()

{
	dynamic_string data;
	std::vector <HRU_record>::iterator hru;

	list_hru->Clear ();
	for (hru = basin->hru_list.hrus.begin ();
	hru != basin->hru_list.hrus.end ();
	++hru) {
		data.format ("%d ", hru->HRU);
		data += hru->Description;
		list_hru->Append (data.get_text_wide ());
	}
}

bool page_subbasin::read_controls ()

{
	strcpy_s (basin->Description, SWAT_DESCRIPTION_FIELD_LENGTH, edit_description->GetValue ().ToAscii ());
	basin->Latitude = edit_latitude->Get ();
	basin->Longitude = edit_longitude->Get ();
	basin->Elevation = edit_elevation->Get ();
	basin->PrecipitationLapseRate = edit_precipitation_lapse_rate->Get ();
	basin->TemperatureLapseRate = edit_temperature_lapse_rate->Get ();
	// basin->Pond = check_pond->GetValue ();
	// basin->Swq = check_swq->GetValue ();
	// basin->WaterUse = check_water_use->GetValue ();
	return true;
}
