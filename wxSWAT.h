#pragma once

#include "Resource.h"

class app_wxswat: public wxApp {
	public:
		dynamic_string AppName;

		short SWATVersion;
		int SWATRevision;
		int number_of_threads;

		// 2021-11-09 separate folders from executable name, to match what swat_controller needs
		std::vector <dynamic_string> run_folders;
		dynamic_string filename_swat_executable [NUMBER_OF_SWAT_VERSIONS];

		bool write_output_hru_annual, write_output_hru_monthly, write_output_hru_daily,
		write_output_subbasin_annual, write_output_subbasin_monthly, write_output_subbasin_daily,
		write_output_reach_annual, write_output_reach_monthly, write_output_reach_daily;
		bool read_output_mgt, use_id, match_file_precision;
		bool write_output_hsl; // Determines if read_swat_hydrology_output will be called.
		BYTE historical_weather_method;

		// Reach #s to save daily output for
		std::set <long> save_daily_reach;

		// HRU #s to save output for, used to set IPDHRU
		std::set <std::pair <long, long>> save_daily_hru;

		bool write_input_std, write_output_std;

		dynamic_string filename_swat_output_variables, filename_swat_help, path_historical_weather;

		// bool run_swat_in_visible_console;
		BYTE swat_run_method;

		std::vector <odbc_database_credentials> db_credentials;
		bool get_credentials
			(const char *filename,
			const BYTE type,
			class odbc_database_credentials *);
		void add_credentials
			(const class odbc_database_credentials &);
		std::vector <odbc_database_credentials>::iterator match_credentials
			(const dynamic_string &filename,
			const BYTE type);

		void load_configuration ();
		void save_configuration ();

		virtual bool OnInit();
		virtual int OnExit ();
		void setup_controller
			(class swat_controller *boss);
};


DECLARE_APP(app_wxswat)