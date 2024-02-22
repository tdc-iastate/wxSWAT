#pragma once

const int MRB_EVALDATA_OTHER_COUNT = 43;
const int MRB_GENOME_LENGTH = 3049;

extern const char * MRB_EVALDATA_OTHER_NAME [MRB_EVALDATA_OTHER_COUNT];

// Functions copied from objective.cpp, used by objective_MRB

extern bool read_point_sources_and_ravine_areas
	(const dynamic_string &filename_db,
	class list_point_source_lookup *point_source_lookup,
	class list_RavineAreas *ravine_areas,
	class interface_window *view,
	dynamic_string &log);

void set_ravine_load_reduction
	(std::map <long, double> *RavineLoad_reduction_Mg_yr,
	int genome [],
	class SWATControlRecord *control,
	class list_RavineAreas *ravine_areas,
	class list_point_source_lookup *point_source_lookup,
	double evaldata_other [],
	dynamic_string &log);

extern bool flow_to_point_sources
	(class SWATControlRecord *control,
	std::map <long, double> *nc_sediment_by_subbasin_Mg_yr,
	std::vector <SWATRouting> *raw_routing_list,
	class list_point_source_lookup *point_source_lookup,
	double evaldata_other [],
	const bool verbose,
	dynamic_string &log);

extern bool apply_RAMO_reduction
	(SWATControlRecord *control,
	std::map <long, double> *RavineLoad_reduction_Mg_yr, // SeJong's RAMO reduction where activated by genome
	std::map <long, double> *nc_sediment_by_subbasin_Mg_yr, // Baseline avarage annual near-channel sediment
	std::vector <SWATRouting> *raw_routing_list,
	double evaldata_other [],
	const bool verbose,
	dynamic_string &log);
void count_wetlands
	(const class SWATControlRecord *control_copy,
	int genome[],
	double evaldata_other[]);

bool sum_first_swat
	(class SWATControlRecord *control,
	const long outlet_subbasin,
	double evaldata_other[],
	const bool verbose,
	dynamic_string &log);
void sum_MRB
	(const class SWATControlRecord *control_copy,
	const long outlet_subbasin,
	int genome [],
	double evaldata_result [],
	double evaldata_other [],
	const bool verbose,
	dynamic_string &error_list);

bool write_recday
	(const class SWATControlRecord *control,
	const class swat_controller *boss,
	const dynamic_string &whereis_swat,
	dynamic_string &error_message);



