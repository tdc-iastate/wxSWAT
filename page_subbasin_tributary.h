
class page_subbasin_tributary : public wxPanel {
	public:
		page_subbasin_tributary
			(wxWindow *parent,
			const int id,
			class SubBasin *);

		void set_controls ();
		bool read_controls ();

		float_edit *edit_initial_snow_water_content;
		float_edit *edit_tributary_channel_longest_length;
		float_edit *edit_tributary_channel_average_slope;
		float_edit *edit_tributary_channel_average_width;
		float_edit *edit_tributary_channel_hydraulic_conductivity;
		float_edit *edit_tributary_channel_mannings_n_coefficient;
		float_edit *edit_carbon_dioxide_concentration;
		wxTextCtrl *edit_help;

	protected:
		class SubBasin *basin;

		DECLARE_EVENT_TABLE()
};

enum {
	EDIT_INITIAL_SNOW_WATER_CONTENT = 101,
	EDIT_TRIBUTARY_CHANNEL_LONGEST_LENGTH,
	EDIT_TRIBUTARY_CHANNEL_AVERAGE_SLOPE,
	EDIT_TRIBUTARY_CHANNEL_AVERAGE_WIDTH,
	EDIT_TRIBUTARY_CHANNEL_HYDRAULIC_CONDUCTIVITY,
	EDIT_TRIBUTARY_CHANNEL_MANNINGS_N_COEFFICIENT,
	EDIT_CARBON_DIOXIDE_CONCENTRATION,
	EDIT_HELP_TRIBUTARY
};
