
#include "../util/utility_afx.h"
#include <chrono>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <thread>
#include <future>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/propdlg.h>
#include <wx/filename.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/dialog.h>
#include <wx/log.h>
#include <wx/app.h>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/stopwatch.h>
#include <wx/sizer.h>
#include <sys/timeb.h>
#include "../util/list.h"
#include "../util/dynamic_string.h"
#include "../util/filename_struct.h"
#include "../util/Timestamp.h"
#include "../util/interface_window.h"
#include "../util/device_coordinate.h"
#include "../util/utility.h"
#include "../util/bounding_cube.h"
#include "../map/color_set.h"
#include "../map/dynamic_map.h"
// wx setup.h must appear before odbc_inst.h or error with HAVE_UNISTD_H
#include <odbcinst.h> // use the Odbcinst.lib import library. Also, Odbccp32.dll must be in the path at run time (or Odbcinst.dll for 16 bit).
#include "../odbc/odbc_database.h"
#include "../odbc/odbc_database_access.h"
#include "../odbc/odbc_database_mysql.h"
#include "../odbc/odbc_database_sql_server.h"
#include "../odbc/odbc_field_set.h"
#include "../util/ARMS_Declarations.h"
#include "../util/nri_point.h"
#include "../i_epic_database/EPICDeclarations.h"
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
#include "../i_swat_database/SWATOutputHRUDaily.h"
#include "../i_swat_database/SWATOutputReachAnnual.h"
#include "../i_swat_database/SWATOutputReachMonthly.h"
#include "../i_swat_database/SWATOutputReachDaily.h"
#include "../i_swat_database/SWATOutputSubBasinAnnual.h"
#include "../i_swat_database/SWATOutputSubBasinMonthly.h"
#include "../i_swat_database/SWATOutputSubBasinDaily.h"
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
#include "../i_swat_database/output_yield_tracker.h"
#include "../i_swat_database/FortranCommonData.h"
#ifdef _MRB_DOUBLE_RUN
#include "../wxGeneticiSWAT/RavineAreas.h"
#include "../wxGeneticiSWAT/PointSourceLookup.h"
#include "MRB_Double_Run.h"


const char * MRB_EVALDATA_OTHER_NAME [MRB_EVALDATA_OTHER_COUNT] = {
	"PND Lake small",
	"PND Lake med",
	"PND Lake large",
	"PND Marsh small",
	"PND Marsh med",
	"PND Marsh large",
	"RES Lake 25 ha",
	"RES Lake 50 ha",
	"RES Lake 250 ha",
	"RES Lake 500 ha",
	"RES Lake 1000 ha",
	"RES Lake 2000 ha",
	"RES Marsh 25 ha",
	"RES Marsh 50 ha",
	"RES Marsh 250 ha",
	"RES Marsh 500 ha",
	"RES Marsh 1000 ha",
	"RES Marsh 2000 ha", // [17]

	"RAMO area m^2",
	"RAMO sediment reduced Mg/yr",
	"SWAT (1st) Outlet Average Daily FlowOut m^3/sec",
	"SWAT (1st) Weighted WYLD mm",
	"SWAT (1st) LeSueur NO3 kg/yr",
	"SWAT (1st) Reach Outlet NO3 kg/yr",
	"SWAT (1st) Reach Outlet P kg/yr",
	"SWAT (1st) Reach Outlet Sediment tons/yr",
	"SWAT (1st) Sediment Transport Subbasin tons/yr",
	"Point Source Sediment tons/yr",
	"Point Source Mineral P kg/yr",
	"Adjusted Point Source Sediment tons/yr",
	"Adjusted Point Source Mineral P kg/yr",
	"SWAT (2nd) Outlet Average Daily FlowOut m^3/sec",
	"SWAT (2nd) WYLD m^3",
	//"SWAT (2nd) LeSueur NO3 kg/yr",
	"SWAT (2nd) Outlet Reach P kg/yr",
	"SWAT (2nd) Sediment Transport Subbasin tons/yr", // [35]
	"SWAT (2nd) NO3 YIELD (SQ) kg/ha",
	"SWAT (2nd) NO3 YIELD (LAT) kg/ha",
	"SWAT (2nd) NO3 YIELD (TILE) kg/ha",
	"SWAT (1st) HRU Sediment Transport HRU 1300 t/ha",
	"SWAT (1st) HRU NO3 Leached HRU 1300 kg/ha",
	"SWAT (2nd) HRU Sediment Transport HRU 1300 t/ha",
	"SWAT (2nd) HRU NO3 Leached HRU 1300 kg/ha"
};

swat_point_source_daily *find_point_source
	(const long subbasin_id,
	SWATControlRecord *control,
	std::vector <SWATRouting> *raw_routing_list,
	dynamic_string &log)

// From module objective.cpp
// This subbasin appears in Brent's PointSourceLookup table, so it must have a point source file

{
	bool error = false;
	std::vector <SWATRouting>::iterator node_entry;
	long point_source_number;
	swat_routing_node_recday *recday;
	swat_point_source_daily *match = NULL;

	for (node_entry = raw_routing_list->begin ();
	!error
	&& (match == NULL)
	&& (node_entry != raw_routing_list->end ());
	++node_entry) {
		if (node_entry->Command == SWAT_ROUTING_COMMAND_RECALL_DAY) {
			point_source_number = atol (node_entry->Filename1);  // "48p.dat"

			if (point_source_number == subbasin_id) {
				if ((recday = (swat_routing_node_recday *) control->routing.match_hsl (node_entry->HydrographStorageLocation)) != NULL) {
					match = &recday->point_source_data;
				}
				else {
					error = true;
					log.add_formatted ("ERROR, HSL %ld not found in routing tree.\n", node_entry->HydrographStorageLocation);
				}
			}
		}
	}
	if (!error) {
		return match;
	}
	else
		return NULL;
}

bool flow_to_point_sources
	(SWATControlRecord *control,
	std::map <long, double> *nc_sediment_by_subbasin_Mg_yr,
	std::vector <SWATRouting> *raw_routing_list,
	class list_point_source_lookup *point_source_lookup,
	double evaldata_other [],
	const bool verbose,
	dynamic_string &log)

// From module objective.cpp
// "r:\swat\MinnesotaRiver\Received 2019-01-22\PointSourceFiles.zip\PointSourceFiles\PointSourceFileGenerator_FWF_MRB_5_Sediment.R"

// Take SWAT flow output from OUTPUT.RCH

// create new point source files

{
	std::map <long, swat_output_reach>::const_iterator output_rch;
	std::vector <SWATOutputReachDaily>::const_iterator reach_day;
	std::map <long, PointSourceLookup>::const_iterator point_source_iterator;
	const PointSourceLookup *lookup;
	swat_point_source_daily *daily_point_sources;
	std::vector <SWATPointSourcesDaily>::iterator source_day;
	double WYLDmm, basin_area_km2, total_area_km2, NC_sediment_tons, NC_P_kg, total_NC_sediment_tons, total_NC_P_kg; // WYLD_m3
	int total_day_count;
	// std::tuple <double, double, double> wyld_sediment_p;
	bool error = false;
	SubBasin *basin;

	total_day_count = 0;

	total_area_km2 = total_NC_sediment_tons = total_NC_P_kg = 0;

	for (output_rch = control->output.reach_map.begin ();
	!error
	&& (output_rch != control->output.reach_map.end ());
	++output_rch) {
		basin = control->match_subbasin (output_rch->second.Reach);
		basin_area_km2 = basin->total_area_km2 ();

		NC_sediment_tons = NC_P_kg = 0.0;

		// Find point source in the routing that corresponds to subbasin/reach
		if ((daily_point_sources = find_point_source (output_rch->second.Reach, control, raw_routing_list, log)) != NULL) {
			if ((point_source_iterator = point_source_lookup->by_subbasin.find (output_rch->second.Reach)) != point_source_lookup->by_subbasin.end ())
				lookup = &point_source_iterator->second;
			else
				lookup = NULL;

			// 2019-12-22 If skip_years > 0, iterate through source_days to find first day that will be part of output
			source_day = daily_point_sources->sources.begin ();
			while (source_day->Year < (control->FirstYear + control->SkipYears))
				++source_day;

			for (reach_day = output_rch->second.day_list.begin ();
			!error
			&& (reach_day != output_rch->second.day_list.end ())
			&& (source_day != daily_point_sources->sources.end ());
			++reach_day, ++source_day) {
				if ((reach_day->Year == source_day->Year)
				&& (julian_day (reach_day->Month, reach_day->Day, is_leap_year (reach_day->Year)) == source_day->JulianDay)) {

					// Adjusted point source sediment & P are enumerated by apply_ramo_reduction.
					// Now set the 'before' values

					NC_sediment_tons += source_day->Sediment;
					NC_P_kg += source_day->MineralP;

					if (lookup) {
						// convert flow from m3 sec-1 to mm day-1
						WYLDmm = ((double) reach_day->FlowOut * 60.0 * 60.0 * 24.0) / (1000.0 * (double) lookup->DrAreaLG_km2);

						// assumes no contribution of near channel sources during low flow
						if (WYLDmm <= lookup->Qthresh)
							WYLDmm = 0.0;

						// this may need updates based on info from Se Jong!!!***
						source_day->Sediment = lookup->a * (float) pow (WYLDmm, lookup->b); // SEDDAY, metric tons

						// total Mg of sediment over incised length per day
						source_day->Sediment *= lookup->Lincised_km;

						// Round to 3 digits
						// point_source->Sediment = (floor (point_source->Sediment * 1000.0 + 0.5)) / 1000.0;

						// bluff sediment P conc = 441 ppm (Sekely et al 2002); SWAT requires kg day-1
						source_day->MineralP = source_day->Sediment * 0.000441f * 1000.0f; // MINPDAY, kg P

						// Round to 3 digits
						// point_source->MineralP = (floor (point_source->MineralP * 1000.0 + 0.5)) / 1000.0;

						// WYLD_m3 += (WYLDmm * basin_area_km2 * 1000.0);
						total_area_km2 += basin_area_km2;

						/*
						if (verbose
						&& (WYLDmm > lookup->Qthresh))
							log.add_formatted ("%04d-%02d-%02d\t%ld\t%.6lf\t%.6lf\t%.6lf\t%.6f\t%.6f\n", reach_day->Year, reach_day->Month, reach_day->Day,
							output_sub->Subbasin, basin_area_km2, reach_day->FlowOut, WYLDmm, (*source_day)->Sediment, (*source_day)->MineralP);
						*/
					}
				}
				else {
					error = true;
					log.add_formatted ("ERROR, mismatch in daily point source Year/Day %d-%d vs %d-%d.\n",
						source_day->Year, source_day->JulianDay,
					reach_day->Year, julian_day (reach_day->Month, reach_day->Day, is_leap_year (reach_day->Year)));
				}
			}
			if (reach_day != output_rch->second.day_list.end ()) {
				error = true;
				log.add_formatted ("ERROR, mismatch in reach output Year/Day %d-%d.\n",
				reach_day->Year, julian_day (reach_day->Month, reach_day->Day, is_leap_year (reach_day->Year)));
			}
		}

		// wyld_sediment_p = std::make_tuple (WYLD_m3, NC_sediment_tons, NC_P_kg);
		// nc_wyld_sediment_P->insert (std::pair <long, std::tuple <double, double, double>> (output_rch->second.Reach, wyld_sediment_p));

		// Compute average annual Near-Channel sediment by subbasin, tons/yr
		nc_sediment_by_subbasin_Mg_yr->insert (std::pair <long, double> (output_rch->second.Reach, NC_sediment_tons / double (control->RunLength - control->SkipYears)));

		total_NC_sediment_tons += NC_sediment_tons;
		total_NC_P_kg += NC_P_kg;
	}

	// Sergey: first we can run 1st SWAT and the R function and add up sediment to get annual values.
	// That will give us what I called  QsedNC(flow) in the google doc
	// a single value for average tons/year

	// [27] "Point Source Sediment tons/yr" (before adjustment)
	evaldata_other [27] = total_NC_sediment_tons / double (control->RunLength - control->SkipYears);
	// [28] "Point Source Mineral P kg/yr" (before adjustment)
	evaldata_other [28] = total_NC_P_kg / double (control->RunLength - control->SkipYears);

	return !error;
}

void set_ravine_load_reduction
	(std::map <long, double> *RavineLoad_reduction_Mg_yr,
	int genome [],
	SWATControlRecord *control,
	class list_RavineAreas *ravine_areas,
	class list_point_source_lookup *point_source_lookup,
	double evaldata_other [],
	dynamic_string &log)

// From GenomeDesign_MRB.set_RAMO

// "R:\SWAT\MinnesotaRiver\Received 2018-11-27\MRB_subs_Ravines.xlsx"

// Use subbasin's proportional ravine load from [Ravine Areas] in "R:\SWAT\MinnesotaRiver\2019-01-23\MRB Wetland Costs.accdb"

{
	int allele, gene_index, i;
	double area_m2, total_extent_m2, total_reduction_Mg_yr, subbasin_reduction_Mg_yr;
	std::vector <SubBasin>::const_iterator subbasin;
	RavineAreas *tips;
	std::map <long, PointSourceLookup>::const_iterator point_source_iterator;
	bool error = false;
	int boolean_true_allele = 9; // "R:\SWAT\MinnesotaRiver\sheets\MRB Genome.xlsx"
	i = 3019; // Start of RAMO section is #3020

	/*
	if (evaldata->ID == capsule->inspect_id)
		log += "Subbasin\tArea m^2\tRAMO Cost $\tSediment Reduction Mg/yr\tWatershed\n";
	*/

	total_extent_m2 = total_reduction_Mg_yr = subbasin_reduction_Mg_yr = 0.0;
	
	std::map <long, std::pair <double, double>> SWAT_outputs_by_subbasin;
	std::map <long, std::pair <double, double>>::iterator subbasin_output;

	gene_index = 0;
	for (subbasin = control->subbasins.basins.begin ();
	!error && (subbasin != control->subbasins.basins.end ());
	++subbasin) {
		subbasin_reduction_Mg_yr = 0.0;
		if ((point_source_iterator = point_source_lookup->by_subbasin.find (subbasin->Subbasin)) != point_source_lookup->by_subbasin.end ()) {
			if ((tips = ravine_areas->match (subbasin->Subbasin)) != NULL) {
				allele = genome [gene_index + i];

				if (allele >= boolean_true_allele) {

					area_m2 = tips->RavineArea_m2;

					RavineLoad_reduction_Mg_yr->insert (std::pair <long, double> (subbasin->Subbasin, tips->RavineLoad_Mg_yr));
					subbasin_reduction_Mg_yr += tips->RavineLoad_Mg_yr;
					total_reduction_Mg_yr += tips->RavineLoad_Mg_yr;
					/*
					if (evaldata->ID == capsule->inspect_id) {
						log.add_formatted ("%ld\t%.6lf\t%.6lf\t%.6lf", subbasin->Subbasin, area_m2, subbasin_cost, subbasin_reduction_Mg_yr);
						log += "\t";
						log += tips->MRBWatershed;
						log += "\n";
					}
					*/
				}
				++gene_index;
			}
		}
	}
	// [18] # RAMO area m^2
	evaldata_other [18] += total_extent_m2;
	// [19] # RAMO sediment reduced Mg/yr
	evaldata_other [19] += total_reduction_Mg_yr;

}


bool apply_RAMO_reduction
	(SWATControlRecord *control,
	std::map <long, double> *RavineLoad_reduction_Mg_yr, // SeJong's RAMO reduction where activated by genome
	std::map <long, double> *nc_sediment_by_subbasin_Mg_yr, // Baseline avarage annual near-channel sediment
	std::vector <SWATRouting> *raw_routing_list,
	double evaldata_other [],
	const bool verbose,
	dynamic_string &log)

// 2019-05-21 Compute ratio of Ravine sediment reduction estimate to NC sediment for each subbasin
// Apply the ratio to daily PS values

{
	double RAMO_efficiency = 0.8;
	std::map <long, double>::iterator nc_annual_average, subbasin_reduction, ratio;
	std::map <long, double> RAMO_adjustment_by_sub;
	double ravine_sed_share_annual;
	std::map <long, swat_output_subbasin>::const_iterator output_sub;
	SubBasin *basin;
	swat_point_source_daily *daily_point_sources;
	std::map <long, PointSourceLookup>::const_iterator point_source_iterator;
	bool error = false;
	std::vector <SWATPointSourcesDaily>::iterator source_day;
	double adjusted_NC_sediment_tons, adjusted_NC_P_kg;

	if (verbose)
		log += "\nSubbasin\tRAMO reduction\tNC annual average\travine_sed_share_annual\tfactor\n";
	for (nc_annual_average = nc_sediment_by_subbasin_Mg_yr->begin ();
	nc_annual_average != nc_sediment_by_subbasin_Mg_yr->end ();
	++nc_annual_average) {
		if ((subbasin_reduction = RavineLoad_reduction_Mg_yr->find (nc_annual_average->first)) != RavineLoad_reduction_Mg_yr->end ()) {
			// then we can compute the (fixed) ratio of ravine contributions as
			// ravine_sed_share_annual = (se Jong ravine value) / (nc_average_annual_baseline_sediment)
			ravine_sed_share_annual = subbasin_reduction->second / nc_annual_average->second;

			// 2019-05-22 Sergey: introduce min(ravine_sed_share_annual,1) in the calculation so we don't have a part larger than the whole
			if (ravine_sed_share_annual > 1.0)
				ravine_sed_share_annual = 1.0;
			// RAMO_adjustment_by_sub = (1 - ravine_sed_shar_annual * RAMO_efficiency). This value can modify all daily values of near channel sediment in EA runs when RAMOs are selected. 
			RAMO_adjustment_by_sub.insert (std::pair <int, double> (nc_annual_average->first, 1.0 - (ravine_sed_share_annual * RAMO_efficiency)));

			if (verbose)
				log.add_formatted ("%ld\t%.7lf\t%.7lf\t%.7lf\t%.7lf\n",
				subbasin_reduction->first, subbasin_reduction->second, nc_annual_average->second, ravine_sed_share_annual, 1.0 - (ravine_sed_share_annual * RAMO_efficiency));
		}
	}

	adjusted_NC_sediment_tons = adjusted_NC_P_kg = 0.0;

	// Reduce daily sediment and Mineral P according to subbasin's ratio
	for (output_sub = control->output.subbasin_map.begin ();
	!error
	&& (output_sub != control->output.subbasin_map.end ());
	++output_sub) {
		basin = control->match_subbasin (output_sub->second.Subbasin);
		ratio = RAMO_adjustment_by_sub.find (output_sub->second.Subbasin);
		// Find point source in the routing that corresponds to subbasin/reach
		if ((daily_point_sources = find_point_source (output_sub->second.Subbasin, control, raw_routing_list, log)) != NULL) {
			for (source_day = daily_point_sources->sources.begin ();
			!error
			&& (source_day != daily_point_sources->sources.end ());
			++source_day) {
				if (ratio != RAMO_adjustment_by_sub.end ()) {
					source_day->Sediment *= (float) ratio->second;
					source_day->MineralP *= (float) ratio->second;
				}
				adjusted_NC_sediment_tons += source_day->Sediment; // metric tons per day
				adjusted_NC_P_kg += source_day->MineralP; // kg P/day
			}
		}
	}

	// adjusted by flow_to_point_sources or by RAMO in this function
	// [29] "Adjusted Point Source Sediment tons/yr"
	evaldata_other [29] = adjusted_NC_sediment_tons / double (control->RunLength - control->SkipYears);
	// [30] "Adjusted Point Source Mineral P kg/yr"
	evaldata_other [30] = adjusted_NC_P_kg / double (control->RunLength - control->SkipYears);

	return !error;
}

bool read_point_sources_and_ravine_areas
	(const dynamic_string &filename_db,
	list_point_source_lookup *point_source_lookup,
	list_RavineAreas *ravine_areas,
	interface_window *view,
	dynamic_string &log)

// 2021-02-10 wxGeneticISWAT reads other files from this database, but wxSWAT only needs point_source_lookup

{
	bool error = false;
	odbc_database *db_odbc = new odbc_database_access;
	odbc_database_credentials creds;
	creds.set_database_filename (filename_db);
#ifdef _WIN64
	creds.set_driver_name (ODBC_ACCESS_DEFAULT_DRIVER_64);
#else
	creds.set_driver_name (ODBC_ACCESS_DEFAULT_DRIVER);
#endif
	creds.type = ODBC_ACCESS;
	creds.set_hostname (".");

	// if (((odbc_database_access *) db_odbc)->open (filename, ODBC_ACCESS_DEFAULT_DRIVER, "", "", L"", log))
	if (((odbc_database_access *) db_odbc)->open (&creds, view, log)) {
		if (!point_source_lookup->read (db_odbc, log))
			error = true;
		if (!ravine_areas->read (db_odbc, log))
			error = true;
	}
	else {
		delete db_odbc;
		db_odbc = NULL;
		error = true;
	}

	return !error;
}

bool write_recday
	(const SWATControlRecord *control,
	const swat_controller *boss,
	const dynamic_string &run_folder,
	dynamic_string &error_message)

// 2019-01-24 Second MRB SWAT run, changes only point source data (code stolen from subbasin_effect's thread_work_swat)

{
	bool error = false;

	if (boss->run_method != SWAT_RUN_METHOD_DLL) {
		dynamic_string filename;

		// 2016-07-05 Write Point Source pcs files
		filename = run_folder + "is.fig";
		control->routing.write_swat_fig_spin (filename.get_text_ascii (),
		boss->swat_version, boss->swat_revision,
		ROUTING_ENVIRONMENT_WRITE_FIG_FILE | ROUTING_ENVIRONMENT_WRITE_RECDAY_RENAME,
		run_folder, control, error_message);
	}

	return !error;
}

bool sum_first_swat
	(SWATControlRecord *control,
	const long outlet_subbasin,
	double evaldata_other [],
	const bool verbose,
	dynamic_string &log)

// "r:\swat\MinnesotaRiver\Received 2019-01-22\PointSourceFiles.zip\PointSourceFiles\PointSourceFileGenerator_FWF_MRB_5_Sediment.R"

// Take SWAT flow output from OUTPUT.RCH

{
	double basin_area_km2, total_area_km2, wyld_m3, sediment_tons;
	std::map <long, SWATOutputSubBasinSum> SWAT_outputs_by_subbasin;
	std::map <long, SWATOutputSubBasinSum>::iterator subbasin_output;
	std::map <long, SWATOutputReachSum> SWAT_outputs_by_reach;
	std::map <long, SWATOutputReachSum>::iterator reach_output;
	bool error = false;
	SWATOutputReachSum rch_output;
	SubBasin *basin;

	if (verbose) {
		log += "Reach\tDay Count\tAve Daily Flow Out, m3 sec-1\tReach NO3, kg\tReach Sediment, tons\tOrganic P, kg\tMineral P, kg";
		log += "\tsum_first_swat";
		// log += "\tSubbasin WYLD, m^3\tSubbasin Sediment Transport, tons";
		// log += "\tNC WYLD m ^ 3\tNC Sediment tons\tNC P kg";
		log += "\n";
	}

	SWAT_outputs_by_reach.insert (std::pair <long, SWATOutputReachSum> (outlet_subbasin, rch_output));
	SWAT_outputs_by_reach.insert (std::pair <long, SWATOutputReachSum> (459, rch_output));

	// # days, flow, WYLD, NO3, sediment, P
	control->output.sum_to_reach (control, &SWAT_outputs_by_reach, log);

	if ((reach_output = SWAT_outputs_by_reach.find (459)) != SWAT_outputs_by_reach.end ()) {
		// LeSueur
		// [22] "SWAT (1st) LeSueur NO3 kg/yr"
		evaldata_other [22] = reach_output->second.NO3_kg / double (control->RunLength - control->SkipYears);

		if (verbose)
			// FLOW_OUT, NO3, Sediment, P
			log.add_formatted ("%ld\t%.6lf\t%.6lf\t%.6lf\t%.6lf\t%.6lf\n",
			reach_output->first,
			reach_output->second.FLOW_m3_second / (double) reach_output->second.record_count,  // FLOW_OUTcms / day count
			reach_output->second.NO3_kg, // NO3_OUTkg
			reach_output->second.Sediment_tons, // SED_OUTtons
			reach_output->second.Organic_P_kg, // ORGP_OUTkg
			reach_output->second.Mineral_P_kg); // MINP_OUTkg
	}
	if ((reach_output = SWAT_outputs_by_reach.find (outlet_subbasin)) != SWAT_outputs_by_reach.end ()) {
		// [20] "SWAT (1st) Outlet Average Daily FlowOut m^3/sec"
		evaldata_other [20] = reach_output->second.FLOW_m3_second / (double) reach_output->second.record_count;
		// [23] "SWAT (1st) Reach Outlet NO3 kg/yr";
		evaldata_other [23] = reach_output->second.NO3_kg / double (control->RunLength - control->SkipYears);
		// [24] "SWAT (1st) Reach Outlet P kg/yr"
		evaldata_other [24] = (reach_output->second.Organic_P_kg + reach_output->second.Mineral_P_kg)
		/ double (control->RunLength - control->SkipYears);
		// [25] SWAT (1st) Reach Outlet Sediment tons/yr";
		evaldata_other [25] = reach_output->second.Sediment_tons / double (control->RunLength - control->SkipYears);
		if (verbose)
			// FLOW_OUT, NO3, Sediment, P
			log.add_formatted ("%ld\t%.6lf\t%.6lf\t%.6lf\t%.6lf\t%.6lf\n",
			reach_output->first,
			reach_output->second.FLOW_m3_second / (double) reach_output->second.record_count,  // FLOW_OUTcms / day count
			reach_output->second.NO3_kg, // NO3_OUTkg
			reach_output->second.Sediment_tons, // SED_OUTtons
			reach_output->second.Organic_P_kg, // ORGP_OUTkg
			reach_output->second.Mineral_P_kg); // MINP_OUTkg
	}

	// Sediment Transport Main Channel
	control->output.sum_to_subbasin (control, &SWAT_outputs_by_subbasin);

	total_area_km2 = 0;
	wyld_m3 = sediment_tons = 0.0;

	for (subbasin_output = SWAT_outputs_by_subbasin.begin ();
	subbasin_output != SWAT_outputs_by_subbasin.end ();
	++subbasin_output) {
		basin = control->match_subbasin (subbasin_output->first);
		basin_area_km2 = basin->total_area_km2 ();
		total_area_km2 += basin_area_km2;

		// Values for the entire watershed

		// [21] "SWAT (1st) Weighted WYLD mm" (wyld is m^3 at this point)
		wyld_m3 += subbasin_output->second.WYLD_m3 * basin_area_km2; // WYLD m3
		// [26] "SWAT (1st) Sediment Transport Subbasin tons/yr" (subbasin_output is tons at this point)
		sediment_tons += subbasin_output->second.SedimentTransport_tons;

	}

	if (total_area_km2 > 0.0)
		// [21] "SWAT (1st) Weighted WYLD mm"
		evaldata_other [21] = wyld_m3 / total_area_km2;

	// [26] "SWAT (1st) Sediment Transport Subbasin tons/yr"
	evaldata_other [26] = sediment_tons / double (control->RunLength - control->SkipYears);

	{
		std::vector <SWATOutputHRUDaily>::const_iterator hru_day;
		double hru_sediment = 0.0, hru_no3 = 0.0;
		std::vector <swat_output_hru>::const_iterator output_hru;

		for (int index = 0; index < OUTPUT_HRU_LIST_HASH; ++index) {
			for (output_hru = control->output.hru_list [index].begin ();
			output_hru != control->output.hru_list [index].end ();
			++output_hru) {

				ASSERT ((output_hru->Subbasin == 146) && (output_hru->HRU == 1));

				for (hru_day = output_hru->day_list.begin ();
				hru_day != output_hru->day_list.end ();
				++hru_day) {
					if (hru_day->Year >= control->FirstYear + control->SkipYears) {
						hru_sediment += (double) hru_day->SedimentTransportMainChannel;
						hru_no3 += (double) hru_day->NO3Leached;
					}
				}
			}
		}
		// Annual Average
		// "SWAT (1st) HRU Sediment Transport HRU 1300 t/ha"
		evaldata_other [39] = hru_sediment / double (control->RunLength - control->SkipYears);
		// "SWAT (1st) HRU NO3 Leached HRU 1300 kg/ha"
		evaldata_other [40] = hru_no3 / double (control->RunLength - control->SkipYears);
	}


	return !error;
}

void count_wetlands
	(const SWATControlRecord *control_copy,
	int genome [],
	double evaldata_other [])

{
	// Put genome counts into OtherResults
	int i, section_count;
	int subbasin_count = control_copy->subbasins.basins.size ();

	// Isolated wetlands
	for (i = 0, section_count = 0; i < subbasin_count; ++i, ++section_count)
		if (genome [i] > 4) {
			// WCMO selected
			if (genome [i] <= 6)
				evaldata_other [0] += 1.0; // PND Lake small
			else
				if (genome [i] <= 8)
					evaldata_other [1] += 1.0; // PND Lake, medium
				else
					if (genome [i] <= 10)
						evaldata_other [2] += 1.0; // PND Lake, large
					else
						if (genome [i] <= 12)
							evaldata_other [3] += 1.0; // PND Marsh, small
						else
							if (genome [i] <= 14)
								evaldata_other [4] += 1.0; // PND Marsh, medium
							else
								evaldata_other [5] += 1.0; // PND Marsh, large
		}

	// Reservoirs
	section_count = 0;
	while (section_count < (subbasin_count - 3)) {
		if (genome [i] > 4) {
			// Res selected
			switch (genome [i]) {
				case 5:
					evaldata_other[6] += 1.0; // RES Lake, 25 ha
					break;
				case 6:
					evaldata_other[7] += 1.0; // RES Lake, 50 ha
					break;
				case 7:
					evaldata_other[8] += 1.0; // RES Lake, 250 ha
					break;
				case 8:
					evaldata_other[9] += 1.0; // RES Lake, 500 ha
					break;
				case 9:
					evaldata_other[10] += 1.0; // RES Lake, 1000 ha
					break;
				case 10:
					evaldata_other[11] += 1.0; // RES Lake, 2000 ha
					break;
				case 11:
					evaldata_other[12] += 1.0; // RES Marsh, 25 ha
					break;
				case 12:
					evaldata_other[13] += 1.0; // RES Marsh, 50 ha
					break;
				case 13:
					evaldata_other[14] += 1.0; // RES Marsh, 250 ha
					break;
				case 14:
					evaldata_other[15] += 1.0; // RES Marsh, 500 ha
					break;
				case 15:
					evaldata_other[16] += 1.0; // RES Marsh, 100 ha
					break;
				case 16:
					evaldata_other[17] += 1.0; // RES Marsh, 2000 ha
			}
		}
		++i;
		++section_count;
	}
}

void sum_MRB
	(const SWATControlRecord *control_copy,
	const long outlet_subbasin,
	int genome [],
	double evaldata_result [],
	double evaldata_other [],
	const bool verbose,
	dynamic_string &error_list)

// Final results from 2nd run

{
	// RCH "NO3 Out" at outlet, skip first two years
	// Nitrate at outlet
	std::map <long, SWATOutputSubBasinSum> SWAT_outputs_by_subbasin; // Sediment transport, WYLD
	std::map <long, SWATOutputSubBasinSum>::iterator subbasin_output;
	std::map <long, SWATOutputReachSum> SWAT_outputs_by_reach; // #days, FLOW_OUT, NO3, Sediment, P
	std::map <long, SWATOutputReachSum>::iterator reach_output;
	SWATOutputReachSum rch_output;
	double basin_area_km2, total_area_km2;
	SubBasin *basin;

	if (verbose) {
		error_list += "ID\tReach\tAve Flow Out, m3 sec-1\tReach NO3, kg\tReach Sediment, tons\tOrganic P, kg\tMineral P, kg\t(sum_MRB)";
		// error_list += "\tSubbasin WYLD, m^3\tSubbasin Sediment Transport, tons";
		error_list += "\n";
	}

	SWAT_outputs_by_reach.insert (std::pair <long, SWATOutputReachSum> (outlet_subbasin, rch_output));
	SWAT_outputs_by_reach.insert (std::pair <long, SWATOutputReachSum> (459, rch_output));

	// Traverse reach output for FLOW_m3_second, NO3_kg, Sediment_tons, Mineral_P_kg, Organic_P_kg;
	control_copy->output.sum_to_reach (control_copy, &SWAT_outputs_by_reach, error_list);

	if ((reach_output = SWAT_outputs_by_reach.find (459)) != SWAT_outputs_by_reach.end ()) {
		// LeSueur
		// [33] "SWAT (2nd) LeSueur NO3 kg/yr"
		evaldata_other [33] += reach_output->second.NO3_kg /  double (control_copy->RunLength - control_copy->SkipYears);
		if (verbose) {
			// FLOW_OUT, NO3, Sediment, P
			error_list.add_formatted ("%ld\t%ld\t%.6lf\t%.6lf\t%.6lf\t%.6lf\t%.6lf\n",
			control_copy->ID,
			reach_output->first,
			reach_output->second.FLOW_m3_second / (double) reach_output->second.record_count,  // FLOW_OUTcms / day count
			reach_output->second.NO3_kg, // NO3_OUTkg
			reach_output->second.Sediment_tons, // SED_OUTtons
			reach_output->second.Organic_P_kg, // ORGP_OUTkg
			reach_output->second.Mineral_P_kg); // MINP_OUTkg
		}
	}
	if ((reach_output = SWAT_outputs_by_reach.find (outlet_subbasin)) != SWAT_outputs_by_reach.end ()) {
		// "Reach Outlet NO3 kg/yr"
		evaldata_result [1] += reach_output->second.NO3_kg /  double (control_copy->RunLength - control_copy->SkipYears);
		// "Reach Outlet Sediment tons/yr"
		evaldata_result [2] += reach_output->second.Sediment_tons / double (control_copy->RunLength - control_copy->SkipYears);
		// [31] "SWAT (2nd) Outlet Average Daily FlowOut m^3/sec";
		evaldata_other [31] += reach_output->second.FLOW_m3_second / (double) reach_output->second.record_count;
		// [34] "SWAT (2nd) Outlet Reach P kg/yr"
		evaldata_other [34] += (reach_output->second.Organic_P_kg + reach_output->second.Mineral_P_kg) / double (control_copy->RunLength - control_copy->SkipYears);

		if (verbose)
			// FLOW_OUT, NO3, Sediment, P
			error_list.add_formatted ("%ld\t%ld\t%.6lf\t%.6lf\t%.6lf\t%.6lf\t%.6lf\n",
			control_copy->ID,
			reach_output->first,
			reach_output->second.FLOW_m3_second / (double) reach_output->second.record_count,  // FLOW_OUTcms / day count
			reach_output->second.NO3_kg, // NO3_OUTkg
			reach_output->second.Sediment_tons, // SED_OUTtons
			reach_output->second.Organic_P_kg, // ORGP_OUTkg
			reach_output->second.Mineral_P_kg); // MINP_OUTkg
	}

	// Traverse subbasin output for WYLD and Sediment transport main channel
	control_copy->output.sum_to_subbasin (control_copy, &SWAT_outputs_by_subbasin);

	total_area_km2 = 0.0;
	for (subbasin_output = SWAT_outputs_by_subbasin.begin ();
	subbasin_output != SWAT_outputs_by_subbasin.end ();
	++subbasin_output) {
		basin = control_copy->match_subbasin (subbasin_output->first);
		basin_area_km2 = basin->total_area_km2 ();
		total_area_km2 += basin_area_km2;

		// [32] "SWAT (2nd) WYLD m^3"
		evaldata_other [32] += subbasin_output->second.WYLD_m3 * basin_area_km2; // WYLDmm converted to m^3

		// [35] "SWAT (2nd) Sediment Transport Subbasin tons/yr"
		evaldata_other [35] += subbasin_output->second.SedimentTransport_tons; // SYLD t/ha, already converted to 'SYLD tons'

		/*
		if (verbose) {
			// FLOW_OUT, NO3, Sediment, P
			error_list.add_formatted ("%ld\t%.6lf\t%.6lf\t%.6lf\t%.6lf",
			reach_output->first,
			reach_output->second.FLOW_m3_second / (double) reach_output->second.subbasin_day_count,  // FLOW_OUTcms / day count
			reach_output->second.NO3_kg, // NO3_OUTkg
			reach_output->second.Sediment_tons, // SED_OUTtons
			reach_output->second.Organic_P_kg, // ORGP_OUTkg
			reach_output->second.Mineral_P_kg); // MINP_OUTkg

			// WYLD m^3, Sediment transport SYLD tons
			error_list.add_formatted ("\t%.6lf\t%.6lf",
			subbasin_output->second.WYLD_m3, subbasin_output->second.SedimentTransport_tons);

			log += "\n";
		}
		*/
	}

	// [32] "SWAT (2nd) WYLD m^3"
	evaldata_other [32] /= total_area_km2; 
	// [35] "SWAT (2nd) Sediment Transport Subbasin tons/yr"
	evaldata_other [35] /= double (control_copy->RunLength - control_copy->SkipYears);

	count_wetlands (control_copy, genome, evaldata_other);

	{
		std::vector <swat_output_hru>::const_iterator output_hru;
		double hru_sediment = 0.0, hru_no3 = 0.0;
		std::vector <SWATOutputHRUDaily>::const_iterator hru_day;
		for (int index = 0; index < OUTPUT_HRU_LIST_HASH; ++index) {
			for (output_hru = control_copy->output.hru_list [index].begin ();
			output_hru != control_copy->output.hru_list [index].end ();
			++output_hru) {
				ASSERT ((output_hru->Subbasin == 146) && (output_hru->HRU == 1));
				for (hru_day = output_hru->day_list.begin ();
				hru_day != output_hru->day_list.end ();
				++hru_day) {
					if (hru_day->Year >= control_copy->FirstYear + control_copy->SkipYears) {
						hru_sediment += (double) hru_day->SedimentTransportMainChannel;
						hru_no3 += (double) hru_day->NO3Leached;
					}
				}
			}
		}
		// "SWAT (2nd) HRU NO3 Leached HRU 1299 kg/ha"
		// Annual Average
		evaldata_other [41] = hru_sediment / double (control_copy->RunLength - control_copy->SkipYears);
		evaldata_other [42] = hru_no3 / double (control_copy->RunLength - control_copy->SkipYears);
	}
}

#endif