const int ID_RADIO_FIXED = 101;
const int ID_RADIO_VARIABLE = 102;
const int BUTTON_FIRST_ROW_LABELS = 103;
const int BUTTON_ADVANCED_CONTROLS = 104;

class page_control_record : public wxPanel {
	public:
		page_control_record
			(wxWindow *parent,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *,
			class odbc_database *);

		wxTextCtrl *edit_description;
		integer_static *static_id;
		integer_edit *edit_run_length;
		integer_edit *edit_first_year;
		integer_edit *edit_first_julian_day;
		integer_edit *edit_last_julian_day;
		integer_edit *edit_stations_per_file_1;
		integer_edit *edit_stations_per_file_2;
		integer_edit *edit_skip_years;
		double_static *static_area;
		wxListBox *list_subbasins;
		wxListBox *list_routes;
		wxComboBox *combo_print_code;
		wxCheckBox *check_condense_std_output;
		wxCheckBox *check_streamflow_logarithm;
		wxCheckBox *check_print_pesticide_output;

		virtual bool TransferDataToWindow ();
		virtual bool TransferDataFromWindow ();

		void OnLanduseReport (wxCommandEvent &);
		void OnBasin (wxCommandEvent &);
		void OnEdit (wxCommandEvent &);
		void OnResizeSubbasins (wxCommandEvent &);
		void OnRenumberHrus (wxCommandEvent &);
		void OnEditRoute (wxCommandEvent &);
		void OnDblClkRoute (wxCommandEvent &);
		void OnDblClickSubbasin (wxCommandEvent& e);
		void OnFirstRowLabels (wxCommandEvent& event);

	protected:
		class SWATControlRecord *control;
		class swat_controller *controller;
		std::vector <long> routing_data;  // persistent for life of page_control_record
		class list_field_help_swat *help_list;
		class odbc_database *db;

		void show_subbasins ();
		void show_routes ();

		DECLARE_EVENT_TABLE()
};

enum {
	BUTTON_LANDUSE_REPORT = 101,
	EDIT_DESCRIPTION,
	STATIC_ID,
	EDIT_RUN_LENGTH,
	EDIT_FIRST_YEAR,
	EDIT_FIRST_JULIAN_DAY,
	EDIT_LAST_JULIAN_DAY,
	EDIT_STATIONS_PER_FILE_1,
	EDIT_STATIONS_PER_FILE_2,
	COMBO_PRINT_CODE,
	EDIT_SKIP_YEARS,
	CHECK_CONDENSE_STD_OUTPUT,
	CHECK_STREAMFLOW_LOGARITHM,
	CHECK_PRINT_PESTICIDE_OUTPUT,
	BUTTON_BASIN,
	STATIC_AREA,
	LIST_SUBBASINS,
	BUTTON_EDIT,
	BUTTON_RESIZE_SUBBASINS,
	BUTTON_RENUMBER_HRUS,
	LIST_ROUTES,
	BUTTON_EDIT_ROUTE
};
