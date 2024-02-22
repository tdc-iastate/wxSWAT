
class page_basin_1 : public wxPanel {
	public:
		enum {
			EDIT_HELP = 101,
			BUTTON_WEATHER_SUMMARY
		};
		page_basin_1
			(wxWindow *parent,
			const int id,
			class SWATControlRecord *,
			class swat_controller *,
			class list_field_help_swat *p_help,
			class odbc_database *p_db);

		virtual bool TransferDataToWindow ();
		virtual bool TransferDataFromWindow ();

		wxButton *check_water_quality, *button_wwq, *check_model_crack_flow;
		wxComboBox	*combo_rainfall_runoff_routing, *combo_potential_evapotranspiration_method;
		float_edit	*edit_snowpacktemperaturelag;
		float_edit	*edit_snowmelttemperature;
		float_edit	*edit_snowmeltratejune21;
		float_edit	*edit_snowmeltratedecember21;
		float_edit	*edit_snowfalltemperature;
		float_edit	*edit_leafareaindexevaporation;
		float_edit	*edit_fieldcapacityinitialsoilwaterfraction;
		float_edit	*edit_esco;
		float_edit	*edit_epco;
		float_edit	*edit_snowcovervolume50percent;
		float_edit	*edit_snowcoverminimumcontent;

		wxTextCtrl *edit_help;

	protected:
		class SWATControlRecord *control;
		class swat_controller *controller;
		class list_field_help_swat *help;
		class odbc_database *db;

		DECLARE_EVENT_TABLE()
};

