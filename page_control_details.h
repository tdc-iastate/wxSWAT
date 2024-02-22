class page_control_details : public wxPanel {
	public:
		page_control_details
			(wxWindow *parent,
			class SWATControlRecord *);

		bool read_controls ();

		wxCheckBox *check_model_instream_nutrients;
		wxCheckBox *check_model_lake_water_quality;
		wxComboBox *combo_special_project_flag;
		wxTextCtrl *edit_help;

	protected:
		class SWATControlRecord *control;
		int records_per_display;

		DECLARE_EVENT_TABLE()
};

enum {
	CHECK_MODEL_INSTREAM_NUTRIENTS = 101,
	CHECK_MODEL_LAKE_WATER_QUALITY,
	COMBO_SPECIAL_PROJECT_FLAG,
	EDIT_HELP
};
