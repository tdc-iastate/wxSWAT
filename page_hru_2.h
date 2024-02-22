
class page_hru_2 : public wxPanel {
	public:
		page_hru_2
			(wxWindow *parent,
			const int id,
			class HRU_record *,
			class SubBasin *,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		virtual bool TransferDataToWindow ();
		virtual bool TransferDataFromWindow ();

		float_edit *edit_initial_residue_cover;
		float_edit *edit_organic_n_enrichment_ratio;
		float_edit *edit_organic_p_enrichment_ratio;
		float_edit *edit_width_of_edge_of_field_filter_strip;
		wxComboBox *combo_urban_simulation_code;
		wxStaticText *static_urban_land_type_id;
		float_edit *edit_sed_con, *edit_orgn_con, *edit_orgp_con, *edit_soln_con, *edit_solp_con;
		wxComboBox *combo_irrigation_code;
		integer_static *static_irrigation_source_location;
		integer_edit *edit_irrigation_source_location;
		float_edit *edit_minimum_instream_flow;
		float_edit *edit_maximum_daily_irrigation_diversion;
		float_edit *edit_fraction_of_available_flow;
		wxTextCtrl *edit_help;

		void OnSetUrbanLandType (wxCommandEvent &);
		void OnSelchangeIrrigationCode (wxCommandEvent &);
		void OnChildFocus (wxChildFocusEvent &);

	protected:
		class HRU_record *hru;
		class SubBasin *subbasin;
		class SWATControlRecord *control;
		class swat_controller *controller;
		class list_field_help_swat *help;
		class odbc_database *database;

		void show_irrigation_source ();

	private:
		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_INITIAL_RESIDUE_COVER = 101,
	EDIT_ORGANIC_N_ENRICHMENT_RATIO,
	EDIT_ORGANIC_P_ENRICHMENT_RATIO,
	EDIT_WIDTH_OF_EDGE_OF_FIELD_FILTER_STRIP,
	COMBO_URBAN_SIMULATION_CODE,
	STATIC_URBAN_LAND_TYPE_ID,
	BUTTON_SET_URBAN_LAND_TYPE,
	COMBO_IRRIGATION_CODE,
	STATIC_IRRIGATION_SOURCE_LOCATION,
	EDIT_IRRIGATION_SOURCE_LOCATION,
	EDIT_MINIMUM_INSTREAM_FLOW,
	EDIT_MAXIMUM_DAILY_IRRIGATION_DIVERSION,
	EDIT_FRACTION_OF_AVAILABLE_FLOW,
	EDIT_HELP_2
};
