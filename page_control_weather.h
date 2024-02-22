class page_control_weather : public wxPanel {
	public:
		page_control_weather
			(wxWindow *parent,
			class SWATControlRecord *);

		// implement wxWizardPage functions
		bool read_controls ();

		integer_edit *edit_random_generator_seed;
		wxComboBox *combo_rainfall_input_code;
		wxComboBox *combo_subdaily_input_timestep;
		wxCheckBox *check_mixed_exponential_distribution;
		float_edit *edit_mixed_exponential_exponent;
		wxComboBox *combo_temperature_input_code;
		wxComboBox *combo_solar_radiation_input_code;
		wxComboBox *combo_relative_humidity_input_code;
		wxComboBox *combo_wind_speed_input_code;
		wxTextCtrl *edit_help;

	protected:
		class SWATControlRecord *control;
		// class gridwidget *grid;
		int records_per_display;

			void fill_output_row
				(const int row_index);
			void show_input ();

		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_RANDOM_GENERATOR_SEED = 101,
	COMBO_RAINFALL_INPUT_CODE,
	COMBO_SUBDAILY_INPUT_TIMESTEP,
	CHECK_MIXED_EXPONENTIAL_DISTRIBUTION,
	EDIT_MIXED_EXPONENTIAL_EXPONENT,
	COMBO_TEMPERATURE_INPUT_CODE,
	COMBO_SOLAR_RADIATION_INPUT_CODE,
	COMBO_RELATIVE_HUMIDITY_INPUT_CODE,
	COMBO_WIND_SPEED_INPUT_CODE,
	EDIT_HELP_WEATHER
};
