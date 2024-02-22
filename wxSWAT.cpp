// wxSide.cpp : Defines the entry point for the application.
//

#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/Timestamp.h"
#include "../util/device_coordinate.h"
#include <wx/app.h>
#include "wx/frame.h"
#include "wx/wizard.h"
#include <wx/filename.h>
#include <wx/listctrl.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/stdpaths.h>
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
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
#include "../i_swat_database/SWATReservoir.h"
#include "../i_swat_database/SWATWeirStage.h"
#include "../i_swat_database/SWATDetentionPond.h"
#include "../i_swat_database/SWATWetPond.h"
#include "../i_swat_database/SWATRetentionIrrigationBasin.h"
#include "../i_swat_database/SWATSedimentationFiltrationBasin.h"
#include "../i_swat_database/SWATPond.h"
#include "../i_swat_database/SWATWaterUse.h"
#include "../i_swat_database/SWATStreamWaterQuality.h"
#include "../i_swat_database/SWATLakeWaterQuality.h"
#include "../i_swat_database/SWATWatershedWaterQuality.h"
#include "../i_swat_database/SWATOutputHSL.h"
#include "../i_swat_database/swat_controller.h"
#include "../i_swat_database/SWATSepticWaterQuality.h"
#include "../i_swat_database/SWATSeptic.h"
#include "../i_epic_database/output_variable_description.h"
#include "../i_swat_database/output_variable_description_swat.h"
#include "wxSWAT.h"
#include "frame_swat.h"
// #include "wizard_control_record.h"

const int MAX_DB_CREDENTIALS = 40;

IMPLEMENT_APP(app_wxswat)

bool app_wxswat::OnInit()
{
	AppName = "wxSWAT";
	if ( !wxApp::OnInit() )
        return false;

	SWATVersion = SWAT_VERSION_2009;
	SWATRevision = 609;
	write_output_hru_annual = true;
	write_output_hru_monthly = false;
	write_output_hru_daily = false;
	write_output_subbasin_annual = true;
	write_output_subbasin_monthly = false;
	write_output_subbasin_daily = false;
	write_output_reach_annual = true;
	write_output_reach_monthly = false;
	write_output_reach_daily = false;
	read_output_mgt = true;
	write_output_hsl = false;
	historical_weather_method = SWAT_HISTORICAL_WEATHER_WRITE;
	use_id = false;
	match_file_precision = true;

	filename_swat_executable [SWAT_VERSION_2000 - 1] = "swat2000.exe";
	filename_swat_executable [SWAT_VERSION_2000M - 1] = "swat2003.exe";
	filename_swat_executable [SWAT_VERSION_2005 - 1] = "swat2005.exe";
	filename_swat_executable [SWAT_VERSION_2007 - 1] = "swat2005.exe";
	filename_swat_executable [SWAT_VERSION_2008 - 1] = "swat2005.exe";
	filename_swat_executable [SWAT_VERSION_2008ARC - 1] = "swat2005.exe";
	filename_swat_executable [SWAT_VERSION_2009 - 1] = "swat2009.exe";

	{
		wxString startup_path = wxStandardPaths::Get ().GetExecutablePath ();
		dynamic_string d = startup_path.ToAscii ();
		run_folders.push_back (d);
	}
	number_of_threads = 1;

	filename_swat_help = "iSWATHelp.csv";
	filename_swat_output_variables = "iSWATOutput.csv";
	// weather_path = "E:\\swat runs\\UMRB\\2017-01-10\\pcp1.pcp"; // SWAT_HISTORICAL_WEATHER_DIRECT_FILE
	// weather_path = "R:\\SWAT\\MinnesotaRiver\\Received 2019-01-22\\Historical Weather\\pcp1.pcp"; // 2019-12-01 MRB
	path_historical_weather = "c:\\swat2012\\"; // 2019-12-01 MRB

	swat_run_method = SWAT_RUN_METHOD_DLL;
	
	load_configuration ();

	frame_swat *frame = new frame_swat ( AppName.get_text (), wxPoint(50, 50), wxSize(150, 300), this );
    frame->Show(true);
    // SetTopWindow(frame);

    /*
	wizard_control_record wizard (frame);
    wizard.RunWizard(wizard.GetFirstPage());
	*/

    return true;
}

int app_wxswat::OnExit ()

{
	save_configuration ();
	return wxApp::OnExit ();
}

void app_wxswat::setup_controller
	(swat_controller *boss)

{
	std::vector <dynamic_string>::const_iterator thread;

	boss->swat_version = SWATVersion;
	boss->swat_revision = SWATRevision;
	boss->run_method = swat_run_method;

	thread = run_folders.begin ();
	boss->run_folder = *thread;
	boss->filename_swat_executable = filename_swat_executable [SWATVersion - 1];
	boss->write_output_hru_annual = write_output_hru_annual;
	boss->write_output_hru_monthly = write_output_hru_monthly;
	boss->write_output_hru_daily = write_output_hru_daily;
	boss->write_output_reach_annual = write_output_reach_annual;
	boss->write_output_reach_monthly =  write_output_reach_monthly;
	boss->write_output_reach_daily = write_output_reach_daily;
	boss->write_output_subbasin_annual = write_output_subbasin_annual;
	boss->write_output_subbasin_monthly = write_output_subbasin_monthly;
	boss->write_output_subbasin_daily = write_output_subbasin_daily;
	boss->imgt = read_output_mgt ? 1 : 0;
	boss->write_output_hsl = write_output_hsl;
	boss->save_daily_reach = save_daily_reach;
	boss->save_daily_hru = save_daily_hru;
	boss->write_input_std = write_input_std;
	boss->write_output_std = write_output_std;
	boss->historical_weather_method = historical_weather_method;
	// memcpy (boss->stations_per_file, ((app_i_swat *) AfxGetApp ())->stations_per_file, sizeof (int) * SWAT_HISTORICAL_WEATHER_FILETYPES);
	boss->use_id = use_id;
	boss->match_file_precision = match_file_precision;
	boss->path_historical_weather_and_atmos = path_historical_weather;
}

void app_wxswat::load_configuration ()

{
	wxConfig *config = new wxConfig (AppName.get_text_wide ());

	wxString str;
	int registry_thread_count, thread_index, version_index, daily_count;
	dynamic_string entry;
	std::vector <dynamic_string> new_folders;
	int config_int;
	long config_long;
	bool version_complete;

	if (config->Read ("filename_swat_output_variables", &str)) {
		filename_swat_output_variables = str.ToAscii ();
		str.clear ();
	}
	if (config->Read ("filename_swat_help", &str)) {
		filename_swat_help = str.ToAscii ();
		str.clear ();
	}
	if (config->Read ("HistoricalWeatherFolder", &str)) {
		path_historical_weather = str.ToAscii ();
		str.clear ();
	}

	if (!config->Read ("Threads", &registry_thread_count))
		registry_thread_count = 1;

	for (version_index = 0; version_index < NUMBER_OF_SWAT_VERSIONS; ++version_index) {
		entry = SWAT_VERSION_NAME[version_index];
		entry.add_formatted ("-%d", version_index + 1);
		if (config->Read (entry.get_text (), &str))
			filename_swat_executable[version_index] = str.ToAscii ();
		else
			// on new machines we end up here
			filename_swat_executable[version_index] = "swat.exe";
	}

	thread_index = 0;
	version_complete = false;
	while (!version_complete) {
		entry.format ("RunFolder-%d", thread_index + 1);
		if (config->Read (entry.get_text (), &str)) {
			entry = str.ToAscii ();
			new_folders.push_back (entry);
		}
		else
			version_complete = true;
		++thread_index;
	}
	// 2022-02-18 If Runfolder-XX doesn't appear in registry, don't clear the single default set to executable path
	if (new_folders.size () > 0)
		run_folders = new_folders;

	number_of_threads = registry_thread_count;

	if (config->Read ("Version", &config_int))
		SWATVersion = (short) config_int;
	if (config->Read ("Revision", &config_int))
		SWATRevision = config_int;

	if (config->Read ("RunMethod", &config_int))
		swat_run_method = (BYTE) config_int;

	if (config->Read ("HistoricalWeatherMethod", &config_int))
		historical_weather_method = (BYTE) config_int;


	/*
	// database credentials for recently used files
	odbc_database_credentials cred;
	int type;
	for (version_index = 0; version_index < MAX_DB_CREDENTIALS; ++version_index) {
		entry.format ("DBType%d", version_index + 1);
		config->Read (entry.get_text (), &type);
		if (type != 0) {
			cred.clear ();
			cred.type = type;
			entry.format ("DBFilename%d", version_index + 1);
			config->Read (entry.get_text (), &str);
			entry = str.ToAscii ();
			cred.set_database_filename (entry);

			entry.format ("DBHostName%d", version_index + 1);
			config->Read (entry.get_text (), &str);
			entry = str.ToAscii ();
			cred.set_hostname (entry);

			entry.format ("DBDriver%d", version_index + 1);
			config->Read (entry.get_text (), &str);
			entry = str.ToAscii ();
			cred.set_driver_name (entry);

			entry.format ("DBUserName%d", version_index + 1);
			config->Read (entry.get_text (), &str);
			entry = str.ToAscii ();
			cred.set_username (entry);

			entry.format ("DBSocket%d", version_index + 1);
			config->Read (entry.get_text (), &str);
			entry = str.ToAscii ();
			cred.set_socket (entry);

			// variable.format ("DBPassword%d", version_index + 1);
			// cred.set_encrypted_password (GetProfileString ("Databases", variable.get_text (), cred.encrypted_password));
			db_credentials.push_back (cred);
		}
	}
	*/
	// output files
	config->Read ("Output HRU Annual", &write_output_hru_annual);
	config->Read ("Output HRU Monthly", &write_output_hru_monthly);
	config->Read ("Output HRU Daily", &write_output_hru_daily);
	config->Read ("Output Subbasin Annual", &write_output_subbasin_annual);
	config->Read ("Output Subbasin Monthly", &write_output_subbasin_monthly);
	config->Read ("Output Subbasin Daily", &write_output_subbasin_daily);
	config->Read ("Output Reach Annual", &write_output_reach_annual);
	config->Read ("Output Reach Monthly", &write_output_reach_monthly);
	config->Read ("Output Reach Daily", &write_output_reach_daily);
	config->Read ("Output MGT", &read_output_mgt);
	config->Read("Output HSL", &write_output_hsl);
	config->Read("Input STD", &write_input_std);
	config->Read("Output STD", &write_output_std);
	config->Read ("Use ID", &use_id);
	config->Read ("Match File Precision", &match_file_precision);

	// Reach numbers to save Daily output for
	if (config->Read ("SaveDailyReachCount", &config_int)) {
		daily_count = config_int;
		for (version_index = 0; version_index < daily_count; ++version_index) {
			entry.format ("DailyReach%d", version_index + 1);
			if (config->Read (entry.get_text (), &config_long)
			&& (config_long != 0))
				save_daily_reach.insert (config_long);
		}
	}

	// HRU numbers to save Daily output for
	if (config->Read ("SaveDailyHRUCount", &config_int)) {
		long sub_id, hru_id;

		daily_count = config_int;
		for (version_index = 0; version_index < daily_count; ++version_index) {
			entry.format ("DailyHRU%d", version_index + 1);

			if (config->Read (entry.get_text (), &str)) {
				entry = str.ToAscii ();
				int position = 0;
				if (entry.parse (" -", &position, &sub_id, true, true)
				&& entry.parse (" -", &position, &hru_id, true, true))
					save_daily_hru.insert (std::pair <long, long> (sub_id, hru_id));
			}
		}
	}

	delete config;
}

void app_wxswat::save_configuration ()

{
	wxConfig *config = new wxConfig (AppName.get_text_wide ());
	wxString str_entry;
	int thread_index, version_index, daily_count;
	dynamic_string entry;
	std::vector <dynamic_string>::iterator swatpath;

	str_entry = filename_swat_output_variables.get_text ();
    config->Write("filename_swat_output_variables", str_entry);
    str_entry = filename_swat_help.get_text ();
    config->Write ("filename_swat_help", str_entry);
	str_entry = path_historical_weather.get_text ();
    config->Write ("HistoricalWeatherFolder", str_entry);

	config->Write ("Threads", number_of_threads);

	for (version_index = 0; version_index < NUMBER_OF_SWAT_VERSIONS; ++version_index) {
		entry = SWAT_VERSION_NAME [version_index];
		entry.add_formatted ("-%d", version_index + 1);
		config->Write (entry.get_text_ascii (), filename_swat_executable [version_index].get_text_ascii ());
	}

	for (swatpath = run_folders.begin (), thread_index = 0;
	swatpath != run_folders.end ();
	++swatpath, ++thread_index) {
		entry.format ("RunFolder-%d", thread_index + 1);
		config->Write (entry.get_text_ascii (), swatpath->get_text_ascii ());
	}

	// Clear out entry for the next thread_index to stop load_config at this point
	entry.format ("RunFolder-%d", thread_index + 1);
	config->DeleteEntry (entry.get_text_ascii ());


	config->Write ("Version", SWATVersion);
	config->Write ("Revision", SWATRevision);
	config->Write ("RunMethod", (int) swat_run_method);
	config->Write ("HistoricalWeatherMethod", (int) historical_weather_method);

	/*
	// database credentials for recently used files
	std::vector <odbc_database_credentials>::iterator it;
	// database credentials for recently used files
	for (it = db_credentials.begin (), version_index = 0;
	it != db_credentials.end ();
	++it, ++version_index) {
		entry.format ("DBType%d", version_index + 1);
		config->Write (entry.get_text (), it->type);

		entry.format ("DBFilename%d", version_index + 1);
		config->Write (entry.get_text (), it->database_name.get_text ());

		entry.format ("DBDriver%d", version_index + 1);
		config->Write (entry.get_text (), it->driver_name.get_text ());

		entry.format ("DBHostName%d", version_index + 1);
		config->Write (entry.get_text (), it->hostname.get_text ());

		entry.format ("DBUserName%d", version_index + 1);
		config->Write (entry.get_text (), it->username.get_text ());

		entry.format ("DBSocket%d", version_index + 1);
		config->Write (entry.get_text (), it->socket.get_text ());
	}
	*/
	// output files
	config->Write ("Output HRU Annual", write_output_hru_annual);
	config->Write ("Output HRU Monthly", write_output_hru_monthly);
	config->Write ("Output HRU Daily", write_output_hru_daily);
	config->Write ("Output Subbasin Annual", write_output_subbasin_annual);
	config->Write ("Output Subbasin Monthly", write_output_subbasin_monthly);
	config->Write ("Output Subbasin Daily", write_output_subbasin_daily);
	config->Write ("Output Reach Annual", write_output_reach_annual);
	config->Write ("Output Reach Monthly", write_output_reach_monthly);
	config->Write ("Output Reach Daily", write_output_reach_daily);
	config->Write ("Output MGT", read_output_mgt);
	config->Write ("Output HSL", write_output_hsl);
	config->Write ("Input STD", write_input_std);
	config->Write ("Output STD", write_output_std);
	config->Write ("Use ID", use_id);
	config->Write ("Match File Precision", match_file_precision);


	// Reach numbers to save Daily output for
	{
		std::set <long>::const_iterator daily_reach;

		daily_count = (int) save_daily_reach.size ();
		config->Write ("SaveDailyReachCount", daily_count);

		for (daily_reach = save_daily_reach.begin (), version_index = 0;
		daily_reach != save_daily_reach.end ();
		++daily_reach, ++version_index) {
			entry.format ("DailyReach%d", version_index + 1);
			config->Write (entry.get_text (), *daily_reach);
		}
	}

	// HRU numbers to save Daily output for
	{
		std::set <std::pair <long, long>>::const_iterator daily_hru;
		dynamic_string pair_text;

		daily_count = (int) save_daily_hru.size ();
		config->Write ("SaveDailyHRUCount", daily_count);

		for (daily_hru = save_daily_hru.begin (), version_index = 0;
		daily_hru != save_daily_hru.end ();
		++daily_hru, ++version_index) {
			entry.format ("DailyHRU%d", version_index + 1);
			pair_text.format ("%ld-%ld", daily_hru->first, daily_hru->second);
			config->Write (entry.get_text (), pair_text.get_text ());
		}
	}

 	delete config;
}

bool app_wxswat::get_credentials
	(const char *filename,
	const BYTE type,
	class odbc_database_credentials *creds)

{
	std::vector <odbc_database_credentials>::iterator it;

	it = match_credentials (filename, type);
	if (it != db_credentials.end ()) {
		creds->copy (*it);
		return true;
	}
	else
		return false;
}

std::vector <odbc_database_credentials>::iterator app_wxswat::match_credentials
	(const dynamic_string &filename,
	const BYTE type)

{
	std::vector <odbc_database_credentials>::iterator it, found;
	found = db_credentials.end ();

	for (it = db_credentials.begin ();
	(found == db_credentials.end ())
	&& (it != db_credentials.end ());
	++it) {
		if ((it->database_name == filename)
		&& (it->type == type))
			found = it;
	}

	return found;
}

void app_wxswat::add_credentials
	(const class odbc_database_credentials &creds)

{
	std::vector <odbc_database_credentials>::iterator it;

	it = match_credentials (creds.database_name, creds.type);

	if (it != db_credentials.end ()) {
		// update existing creds
		it->copy (creds);
	}
	else {
		if (db_credentials.size () > MAX_DB_CREDENTIALS)
			// remove oldest one
			db_credentials.erase (db_credentials.begin ());
		db_credentials.push_back (creds);
	}
}
