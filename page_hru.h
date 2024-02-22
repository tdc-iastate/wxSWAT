
class page_hru : public wxPanel {
	public:
		page_hru
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

		long_integer_edit *edit_hru;
		wxTextCtrl *edit_description;
		wxComboBox *combo_rotation;
		integer_edit *edit_rotation_length;
		float_edit *edit_curve_number;
		wxComboBox *combo_tillage;
		long_integer_edit *edit_location_code;
		wxStaticText *static_soil;
		float_edit *edit_slope;
		float_edit *edit_slope_length;
		float_edit *edit_mannings_n_coefficient;
		float_edit *edit_lateral_flow_travel_time;
		double_edit *edit_fraction_of_total_watershed_area;
		double_static *static_fractionofsubbasin;
		double_static *static_fractionofwatershed;
		wxListBox *list_operations;
		wxTextCtrl *edit_help;

		void OnAddOperation (wxCommandEvent &);
		void OnEditOperation (wxCommandEvent &);
		void OnDeleteOperation (wxCommandEvent &);
		void OnListOps (wxCommandEvent &);
		void OnChildFocus (wxChildFocusEvent &);

	protected:
		class HRU_record *hru;
		class SubBasin *subbasin;
		class SWATControlRecord *control;
		class swat_controller *controller;
		class list_field_help_swat *help;
		class odbc_database *database;

		void show_area_fraction ();
		void show_management ();

	private:
		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_HRU = 101,
	EDIT_DESCRIPTION,
	COMBO_ROTATION,
	EDIT_ROTATION_LENGTH,
	EDIT_CURVE_NUMBER,
	COMBO_TILLAGE,
	EDIT_LOCATION_CODE,
	STATIC_SOIL,
	EDIT_SLOPE,
	EDIT_SLOPE_LENGTH,
	EDIT_MANNINGS_N_COEFFICIENT,
	EDIT_LATERAL_FLOW_TRAVEL_TIME,
	EDIT_FRACTION_OF_TOTAL_WATERSHED_AREA,
	STATIC_FRACTIONOFSUBBASIN,
	STATIC_FRACTIONOFWATERSHED,
	BUTTON_ADD_OPERATION,
	BUTTON_EDIT_OPERATION,
	BUTTON_DELETE_OPERATION,
	BUTTON_LIST_OPS,
	LIST_OPERATIONS,
	EDIT_HELP
};
